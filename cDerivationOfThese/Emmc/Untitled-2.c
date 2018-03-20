#define SD_1_8V_SUPPORT 1

int sd_card_init(struct block_device **dev)
{

#if SDHCI_IMPLEMENTATION == SDHCI_IMPLEMENTATION_BCM_2708
	// Power cycle the card to ensure its in its startup state
	if(bcm_2708_power_cycle() != 0)
	{
		printf("EMMC: BCM2708 controller did not power cycle successfully\n");
	}
	else
		printf("EMMC: BCM2708 controller power-cycled\n");
#endif

	// Reset the controller
	printf("EMMC: resetting controller\n");

	uint32_t control1 = mmio_read(emmc_base + EMMC_CONTROL1);
	control1 |= (1 << 24);
	// Disable clock
	control1 &= ~(1 << 2);
	control1 &= ~(1 << 0);
	mmio_write(emmc_base + EMMC_CONTROL1, control1);
	TIMEOUT_WAIT((mmio_read(emmc_base + EMMC_CONTROL1) & (0x7 << 24)) == 0, 1000000);

	// Check for a valid card

	printf("EMMC: checking for an inserted card\n");

    TIMEOUT_WAIT(mmio_read(emmc_base + EMMC_STATUS) & (1 << 16), 500000);
	uint32_t status_reg = mmio_read(emmc_base + EMMC_STATUS);
	if((status_reg & (1 << 16)) == 0)
	{
		printf("EMMC: no card inserted\n");
		return -1;
	}

	printf("EMMC: status: %08x\n", status_reg);

	// Clear control2
	mmio_write(emmc_base + EMMC_CONTROL2, 0);

	control1 = mmio_read(emmc_base + EMMC_CONTROL1);
	control1 |= 1;			// enable clock

	// Set to identification frequency (400 kHz)
	uint32_t f_id = sd_get_clock_divider(base_clock, SD_CLOCK_ID);
	
	control1 |= f_id;

	control1 |= (7 << 16);		// data timeout = TMCLK * 2^10
	mmio_write(emmc_base + EMMC_CONTROL1, control1);
	TIMEOUT_WAIT(mmio_read(emmc_base + EMMC_CONTROL1) & 0x2, 0x1000000);
	if((mmio_read(emmc_base + EMMC_CONTROL1) & 0x2) == 0)
	{
		printf("EMMC: controller's clock did not stabilise within 1 second\n");
		return -1;
	}

	printf("EMMC: control0: %08x, control1: %08x\n",
			mmio_read(emmc_base + EMMC_CONTROL0),
			mmio_read(emmc_base + EMMC_CONTROL1));

	// Enable the SD clock
	printf("EMMC: enabling SD clock\n");

	usleep(2000);
	control1 = mmio_read(emmc_base + EMMC_CONTROL1);
	control1 |= 4;
	mmio_write(emmc_base + EMMC_CONTROL1, control1);
	usleep(2000);

	printf("EMMC: SD clock enabled\n");

	// Mask off sending interrupts to the ARM
	mmio_write(emmc_base + EMMC_IRPT_EN, 0);
	// Reset interrupts
	mmio_write(emmc_base + EMMC_INTERRUPT, 0xffffffff);
	// Have all interrupts sent to the INTERRUPT register
	uint32_t irpt_mask = 0xffffffff & (~SD_CARD_INTERRUPT);
#ifdef SD_CARD_INTERRUPTS
    irpt_mask |= SD_CARD_INTERRUPT;
#endif
	mmio_write(emmc_base + EMMC_IRPT_MASK, irpt_mask);

	printf("EMMC: interrupts disabled\n");
	usleep(2000);

    // Prepare the device structure
	struct emmc_block_dev *ret;
	if(*dev == NULL)
		ret = (struct emmc_block_dev *)malloc(sizeof(struct emmc_block_dev));
	else
		ret = (struct emmc_block_dev *)*dev;

	assert(ret);

	memset(ret, 0, sizeof(struct emmc_block_dev));
	ret->bd.driver_name = driver_name;
	ret->bd.device_name = device_name;
	ret->bd.block_size = 512;
	ret->bd.read = sd_read;
#ifdef SD_WRITE_SUPPORT
    ret->bd.write = sd_write;
#endif
    ret->bd.supports_multiple_block_read = 1;
    ret->bd.supports_multiple_block_write = 1;
	ret->base_clock = base_clock;

	printf("EMMC: device structure created\n");

	// Send CMD0 to the card (reset to idle state)
	sd_issue_command(ret, GO_IDLE_STATE, 0, 500000);
	if(FAIL(ret))
	{
        printf("SD: no CMD0 response\n");
        return -1;
	}

	// Send CMD8 to the card
	// Voltage supplied = 0x1 = 2.7-3.6V (standard)
	// Check pattern = 10101010b (as per PLSS 4.3.13) = 0xAA

    printf("SD: note a timeout error on the following command (CMD8) is normal "
           "and expected if the SD card version is less than 2.0\n");

	sd_issue_command(ret, SEND_IF_COND, 0x1aa, 500000);
	int v2_later = 0;
	if(TIMEOUT(ret))
        v2_later = 0;
    else if(CMD_TIMEOUT(ret))
    {
        if(sd_reset_cmd() == -1)
            return -1;
        mmio_write(emmc_base + EMMC_INTERRUPT, SD_ERR_MASK_CMD_TIMEOUT);
        v2_later = 0;
    }
    else if(FAIL(ret))
    {
        printf("SD: failure sending CMD8 (%08x)\n", ret->last_interrupt);
        return -1;
    }
    else
    {
        if((ret->last_r0 & 0xfff) != 0x1aa)
        {
            printf("SD: unusable card\n");

            printf("SD: CMD8 response %08x\n", ret->last_r0);

            return -1;
        }
        else
            v2_later = 1;
    }

    // Call an inquiry ACMD41 (voltage window = 0) to get the OCR

    printf("SD: sending inquiry ACMD41\n");

    sd_issue_command(ret, ACMD(41), 0, 500000);
    if(FAIL(ret))
    {
        printf("SD: inquiry ACMD41 failed\n");
        return -1;
    }

    printf("SD: inquiry ACMD41 returned %08x\n", ret->last_r0);

	// Call initialization ACMD41
	int card_is_busy = 1;
	while(card_is_busy)
	{
	    uint32_t v2_flags = 0;
	    if(v2_later)
	    {
	        // Set SDHC support
	        v2_flags |= (1 << 30);

	        // Set 1.8v support
#ifdef SD_1_8V_SUPPORT
	        if(!ret->failed_voltage_switch)
                v2_flags |= (1 << 24);
#endif

	    }

	    sd_issue_command(ret, ACMD(41), 0x00ff8000 | v2_flags, 500000);
		***************************************************************************************
	    if(FAIL(ret))
	    {
	        printf("SD: error issuing ACMD41\n");
	        return -1;
	    }

	    if((ret->last_r0 >> 31) & 0x1)
	    {
	        // Initialization is complete
	        ret->card_ocr = (ret->last_r0 >> 8) & 0xffff;
	        ret->card_supports_sdhc = (ret->last_r0 >> 30) & 0x1;

#ifdef SD_1_8V_SUPPORT
	        if(!ret->failed_voltage_switch)
                ret->card_supports_18v = (ret->last_r0 >> 24) & 0x1;
#endif

	        card_is_busy = 0;
	    }
	    else
	    {
	        // Card is still busy
            printf("SD: card is busy, retrying\n");

            usleep(500000);
	    }
	}

	printf("SD: card identified: OCR: %04x, 1.8v support: %i, SDHC support: %i\n",
			ret->card_ocr, ret->card_supports_18v, ret->card_supports_sdhc);

    // At this point, we know the card is definitely an SD card, so will definitely
	//  support SDR12 mode which runs at 25 MHz
    sd_switch_clock_rate(base_clock, SD_CLOCK_NORMAL);

	// A small wait before the voltage switch
	usleep(5000);

	// Switch to 1.8V mode if possible
	if(ret->card_supports_18v)
	{
        printf("SD: switching to 1.8V mode\n");
	    // As per HCSS 3.6.1

	    // Send VOLTAGE_SWITCH
	    sd_issue_command(ret, VOLTAGE_SWITCH, 0, 500000);
	    if(FAIL(ret))
	    {
            printf("SD: error issuing VOLTAGE_SWITCH\n");
	        ret->failed_voltage_switch = 1;
			sd_power_off();
	        return sd_card_init((struct block_device **)&ret);
	    }

	    // Disable SD clock
	    control1 = mmio_read(emmc_base + EMMC_CONTROL1);
	    control1 &= ~(1 << 2);
	    mmio_write(emmc_base + EMMC_CONTROL1, control1);

	    // Check DAT[3:0]
	    status_reg = mmio_read(emmc_base + EMMC_STATUS);
	    uint32_t dat30 = (status_reg >> 20) & 0xf;
	    if(dat30 != 0)
	    {
            printf("SD: DAT[3:0] did not settle to 0\n");
	        ret->failed_voltage_switch = 1;
			sd_power_off();
	        return sd_card_init((struct block_device **)&ret);
	    }

	    // Set 1.8V signal enable to 1
	    uint32_t control0 = mmio_read(emmc_base + EMMC_CONTROL0);
	    control0 |= (1 << 8);
	    mmio_write(emmc_base + EMMC_CONTROL0, control0);

	    // Wait 5 ms
	    usleep(5000);

	    // Check the 1.8V signal enable is set
	    control0 = mmio_read(emmc_base + EMMC_CONTROL0);
	    if(((control0 >> 8) & 0x1) == 0)
	    {
            printf("SD: controller did not keep 1.8V signal enable high\n");
	        ret->failed_voltage_switch = 1;
			sd_power_off();
	        return sd_card_init((struct block_device **)&ret);
	    }

	    // Re-enable the SD clock
	    control1 = mmio_read(emmc_base + EMMC_CONTROL1);
	    control1 |= (1 << 2);
	    mmio_write(emmc_base + EMMC_CONTROL1, control1);

	    // Wait 1 ms
	    usleep(10000);

	    // Check DAT[3:0]
	    status_reg = mmio_read(emmc_base + EMMC_STATUS);
	    dat30 = (status_reg >> 20) & 0xf;
	    if(dat30 != 0xf)
	    {
            printf("SD: DAT[3:0] did not settle to 1111b (%01x)\n", dat30);
	        ret->failed_voltage_switch = 1;
			sd_power_off();
	        return sd_card_init((struct block_device **)&ret);
	    }

        printf("SD: voltage switch complete\n");
	}

	// Send CMD2 to get the cards CID
	sd_issue_command(ret, ALL_SEND_CID, 0, 500000);
	if(FAIL(ret))
	{
	    printf("SD: error sending ALL_SEND_CID\n");
	    return -1;
	}
	uint32_t card_cid_0 = ret->last_r0;
	uint32_t card_cid_1 = ret->last_r1;
	uint32_t card_cid_2 = ret->last_r2;
	uint32_t card_cid_3 = ret->last_r3;

	printf("SD: card CID: %08x%08x%08x%08x\n", card_cid_3, card_cid_2, card_cid_1, card_cid_0);
	uint32_t *dev_id = (uint32_t *)malloc(4 * sizeof(uint32_t));
	dev_id[0] = card_cid_0;
	dev_id[1] = card_cid_1;
	dev_id[2] = card_cid_2;
	dev_id[3] = card_cid_3;
	ret->bd.device_id = (uint8_t *)dev_id;
	ret->bd.dev_id_len = 4 * sizeof(uint32_t);

	// Send CMD3 to enter the data state
	sd_issue_command(ret, SEND_RELATIVE_ADDR, 0, 500000);
	if(FAIL(ret))
    {
        printf("SD: error sending SEND_RELATIVE_ADDR\n");
        free(ret);
        return -1;
    }

	uint32_t cmd3_resp = ret->last_r0;
	printf("SD: CMD3 response: %08x\n", cmd3_resp);

	ret->card_rca = (cmd3_resp >> 16) & 0xffff;
	uint32_t crc_error = (cmd3_resp >> 15) & 0x1;
	uint32_t illegal_cmd = (cmd3_resp >> 14) & 0x1;
	uint32_t error = (cmd3_resp >> 13) & 0x1;
	uint32_t status = (cmd3_resp >> 9) & 0xf;
	uint32_t ready = (cmd3_resp >> 8) & 0x1;

	if(crc_error)
	{
		printf("SD: CRC error\n");
		free(ret);
		free(dev_id);
		return -1;
	}

	if(illegal_cmd)
	{
		printf("SD: illegal command\n");
		free(ret);
		free(dev_id);
		return -1;
	}

	if(error)
	{
		printf("SD: generic error\n");
		free(ret);
		free(dev_id);
		return -1;
	}

	if(!ready)
	{
		printf("SD: not ready for data\n");
		free(ret);
		free(dev_id);
		return -1;
	}

	printf("SD: RCA: %04x\n", ret->card_rca);

	// Now select the card (toggles it to transfer state)
	sd_issue_command(ret, SELECT_CARD, ret->card_rca << 16, 500000);
	if(FAIL(ret))
	{
	    printf("SD: error sending CMD7\n");
	    free(ret);
	    return -1;
	}

	uint32_t cmd7_resp = ret->last_r0;
	status = (cmd7_resp >> 9) & 0xf;

	if((status != 3) && (status != 4))
	{
		printf("SD: invalid status (%i)\n", status);
		free(ret);
		free(dev_id);
		return -1;
	}
	
	ret->block_size = 512;
	uint32_t controller_block_size = mmio_read(emmc_base + EMMC_BLKSIZECNT);
	controller_block_size &= (~0xfff);
	controller_block_size |= 0x200;
	mmio_write(emmc_base + EMMC_BLKSIZECNT, controller_block_size);

	// Get the cards SCR register
	ret->scr = (struct sd_scr *)malloc(sizeof(struct sd_scr));
	ret->buf = &ret->scr->scr[0];
	ret->block_size = 8;
	ret->blocks_to_transfer = 1;
	sd_issue_command(ret, SEND_SCR, 0, 500000);
	ret->block_size = 512;
	if(FAIL(ret))
	{
	    printf("SD: error sending SEND_SCR\n");
	    free(ret->scr);
        free(ret);
	    return -1;
	}

	// Determine card version
	// Note that the SCR is big-endian
	uint32_t scr0 = byte_swap(ret->scr->scr[0]);
	ret->scr->sd_version = SD_VER_UNKNOWN;
	uint32_t sd_spec = (scr0 >> (56 - 32)) & 0xf;
	uint32_t sd_spec3 = (scr0 >> (47 - 32)) & 0x1;
	uint32_t sd_spec4 = (scr0 >> (42 - 32)) & 0x1;
	ret->scr->sd_bus_widths = (scr0 >> (48 - 32)) & 0xf;
	if(sd_spec == 0)
        ret->scr->sd_version = SD_VER_1;
    else if(sd_spec == 1)
        ret->scr->sd_version = SD_VER_1_1;
    else if(sd_spec == 2)
    {
        if(sd_spec3 == 0)
            ret->scr->sd_version = SD_VER_2;
        else if(sd_spec3 == 1)
        {
            if(sd_spec4 == 0)
                ret->scr->sd_version = SD_VER_3;
            else if(sd_spec4 == 1)
                ret->scr->sd_version = SD_VER_4;
        }
    }

    printf("SD: &scr: %08x\n", &ret->scr->scr[0]);
    printf("SD: SCR[0]: %08x, SCR[1]: %08x\n", ret->scr->scr[0], ret->scr->scr[1]);;
    printf("SD: SCR: %08x%08x\n", byte_swap(ret->scr->scr[0]), byte_swap(ret->scr->scr[1]));
    printf("SD: SCR: version %s, bus_widths %01x\n", sd_versions[ret->scr->sd_version],
           ret->scr->sd_bus_widths);

	// Reset interrupt register
	mmio_write(emmc_base + EMMC_INTERRUPT, 0xffffffff);

	*dev = (struct block_device *)ret;

	return 0;
}

static void sd_issue_command_int(struct emmc_block_dev *dev, uint32_t cmd_reg, uint32_t argument, useconds_t timeout)
{
    dev->last_cmd_reg = cmd_reg;
    dev->last_cmd_success = 0;

    // This is as per HCSS 3.7.1.1/3.7.2.2

    // Check Command Inhibit
    while(mmio_read(emmc_base + EMMC_STATUS) & 0x1)
        usleep(1000);

    // Is the command with busy?
    if((cmd_reg & SD_CMD_RSPNS_TYPE_MASK) == SD_CMD_RSPNS_TYPE_48B)
    {
        // With busy

        // Is is an abort command?
        if((cmd_reg & SD_CMD_TYPE_MASK) != SD_CMD_TYPE_ABORT)
        {
            // Not an abort command

            // Wait for the data line to be free
            while(mmio_read(emmc_base + EMMC_STATUS) & 0x2)
                usleep(1000);
        }
    }

    uint32_t blksizecnt = dev->block_size | (dev->blocks_to_transfer << 16);
    mmio_write(emmc_base + EMMC_BLKSIZECNT, blksizecnt);

    // Set argument 1 reg
    mmio_write(emmc_base + EMMC_ARG1, argument);

    // Set command reg
    mmio_write(emmc_base + EMMC_CMDTM, cmd_reg);

    usleep(2000);

    // Wait for command complete interrupt
    TIMEOUT_WAIT(mmio_read(emmc_base + EMMC_INTERRUPT) & 0x8001, timeout);
    uint32_t irpts = mmio_read(emmc_base + EMMC_INTERRUPT);

    // Clear command complete status
    mmio_write(emmc_base + EMMC_INTERRUPT, 0xffff0001);

    // Test for errors
    if((irpts & 0xffff0001) != 0x1)
    {
#ifdef EMMC_DEBUG
        printf("SD: error occured whilst waiting for command complete interrupt\n");
#endif
        dev->last_error = irpts & 0xffff0000;
        dev->last_interrupt = irpts;
        return;
    }

    usleep(2000);

    // Get response data
    switch(cmd_reg & SD_CMD_RSPNS_TYPE_MASK)
    {
        case SD_CMD_RSPNS_TYPE_48:
        case SD_CMD_RSPNS_TYPE_48B:
            dev->last_r0 = mmio_read(emmc_base + EMMC_RESP0);
            break;

        case SD_CMD_RSPNS_TYPE_136:
            dev->last_r0 = mmio_read(emmc_base + EMMC_RESP0);
            dev->last_r1 = mmio_read(emmc_base + EMMC_RESP1);
            dev->last_r2 = mmio_read(emmc_base + EMMC_RESP2);
            dev->last_r3 = mmio_read(emmc_base + EMMC_RESP3);
            break;
    }

    // If with data, wait for the appropriate interrupt
    if((cmd_reg & SD_CMD_ISDATA) && (is_sdma == 0))
    {
        uint32_t wr_irpt;
        int is_write = 0;
        if(cmd_reg & SD_CMD_DAT_DIR_CH)
            wr_irpt = (1 << 5);     // read
        else
        {
            is_write = 1;
            wr_irpt = (1 << 4);     // write
        }

        int cur_block = 0;
        uint32_t *cur_buf_addr = (uint32_t *)dev->buf;
        while(cur_block < dev->blocks_to_transfer)
        {
#ifdef EMMC_DEBUG
			if(dev->blocks_to_transfer > 1)
				printf("SD: multi block transfer, awaiting block %i ready\n",
				cur_block);
#endif
            TIMEOUT_WAIT(mmio_read(emmc_base + EMMC_INTERRUPT) & (wr_irpt | 0x8000), timeout);
            irpts = mmio_read(emmc_base + EMMC_INTERRUPT);
            mmio_write(emmc_base + EMMC_INTERRUPT, 0xffff0000 | wr_irpt);

            if((irpts & (0xffff0000 | wr_irpt)) != wr_irpt)
            {
#ifdef EMMC_DEBUG
            printf("SD: error occured whilst waiting for data ready interrupt\n");
#endif
                dev->last_error = irpts & 0xffff0000;
                dev->last_interrupt = irpts;
                return;
            }

            // Transfer the block
            size_t cur_byte_no = 0;
            while(cur_byte_no < dev->block_size)
            {
                if(is_write)
				{
					uint32_t data = read_word((uint8_t *)cur_buf_addr, 0);
                    mmio_write(emmc_base + EMMC_DATA, data);
				}
                else
				{
					uint32_t data = mmio_read(emmc_base + EMMC_DATA);
					write_word(data, (uint8_t *)cur_buf_addr, 0);
				}
                cur_byte_no += 4;
                cur_buf_addr++;
            }

#ifdef EMMC_DEBUG
			printf("SD: block %i transfer complete\n", cur_block);
#endif

            cur_block++;
        }
    }

    // Wait for transfer complete (set if read/write transfer or with busy)
    if((((cmd_reg & SD_CMD_RSPNS_TYPE_MASK) == SD_CMD_RSPNS_TYPE_48B) ||
       (cmd_reg & SD_CMD_ISDATA)) && (is_sdma == 0))
    {
        // First check command inhibit (DAT) is not already 0
        if((mmio_read(emmc_base + EMMC_STATUS) & 0x2) == 0)
            mmio_write(emmc_base + EMMC_INTERRUPT, 0xffff0002);
        else
        {
            TIMEOUT_WAIT(mmio_read(emmc_base + EMMC_INTERRUPT) & 0x8002, timeout);
            irpts = mmio_read(emmc_base + EMMC_INTERRUPT);
            mmio_write(emmc_base + EMMC_INTERRUPT, 0xffff0002);

            // Handle the case where both data timeout and transfer complete
            //  are set - transfer complete overrides data timeout: HCSS 2.2.17
            if(((irpts & 0xffff0002) != 0x2) && ((irpts & 0xffff0002) != 0x100002))
            {
#ifdef EMMC_DEBUG
                printf("SD: error occured whilst waiting for transfer complete interrupt\n");
#endif
                dev->last_error = irpts & 0xffff0000;
                dev->last_interrupt = irpts;
                return;
            }
            mmio_write(emmc_base + EMMC_INTERRUPT, 0xffff0002);
        }
    }

}
