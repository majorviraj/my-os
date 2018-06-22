#include <assemblyFunctions.h>
#include <intTypes.h>
#include <stdOutput.h>
#include <libfatfs.h>
#include <lib_bmp.h>

/*
* Load the bmp_header struct from the file read.
* render the image to a given coordinate.
* x & y coordinates given are where the bottom left of the image will be placed.
*/

void renderBmp(uint8_t* file, uint32_t fileSize, uint16_t x, uint16_t y) {
	bmp_header_t bmp_header;
	memcpy(&bmp_header, file, 54);
	
	//Print BMP Header info to check if file was properly loaded.
	printf("BMP INFO***********\nIdentifier: %c", bmp_header.bmpIdentifier[0]);	
	printf("%c", bmp_header.bmpIdentifier[1]);
	printf("BMP File Size: %c", bmp_header.bmpSize);	
	printf("BMP Width: %c", bmp_header.bmpWidth);
	printf("BMP Height: %c", bmp_header.bmpHeight);	
	printf("BMP ColourDepth: %c", bmp_header.bmpColourDepth);
	printf("BMP CompressionType: %c\n", bmp_header.bmpCompressionType);

	//Now extract the actual raw image data from the file into a new aray which conforms to the bmp standar.
	//BMP files store raw data in 32 bit DWORDS. So it expects no. of bytes in a row to be a multiple of 4

	//rowSize in bytes = floor[(bitsPerPixel*imageWidth + 31) / 32] * 4;
	uint16_t rowSize = (uint16_t)((bmp_header.bmpColourDepth * bmp_header.bmpWidth + 31)/32) * 4;
	uint32_t rawImageArraySize = rowSize * bmp_header.bmpHeight;

	//now iterate the "file" array from bmpImageOffset for rawImageArraySize times and call setPixel.
	//NOTE the bmp style of storing pixel values with padding to fill up rows of 32bits.
	
	uint16_t originalX = x;

	for(uint32_t k = 0; k < bmp_header.bmpHeight; k++) {
		for(uint32_t i = 0; i < bmp_header.bmpWidth; i++) {
			//Check little-endinness or big-endiannes if the code does not work.
			setForeColour(file[k*rowSize + i*2 + bmp_header.bmpImageOffset] 
							& (file[k*rowSize + (i*2 + 1) + bmp_header.bmpImageOffset]<<8) );
			drawPixel(x, y);
			x += 1; 
		}
		y += 1;
		x = originalX;
	}
	
}