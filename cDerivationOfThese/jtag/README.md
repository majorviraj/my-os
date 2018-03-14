How to use JTAG debugging with BusPirate on Rpi 
===
( Tested on Rpi B+, should work on all orignal and plus models )

Prerequisites
---
* BusPirate (we have tested it only on BusPirate)
* OpenOCD built with BusPirate enabled
* A compitable Rpi
* Some Jumpers to connect the two
* GDB (optional)

Steps
---
1. Download the latest version of GDB from https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads
(download the full toolchain to get GDB)
or use the version form your distribution's package manager also install git
```
sudo apt-get install gdb-arm-none-eabi git
```

2. Clone the OpenOCD repository found [here](https://sourceforge.net/p/openocd/code/ci/master/tree/) and reset head to the latest relese of OpenOCD I used v0.10.0
```
git clone https://git.code.sf.net/p/openocd/code openocd-code
git reset --hard v0.10.0
```
3. Install dependencies required to compile OpenOCD
```
sudo apt-get install libtool autoconf texinfo libusb-dev libftdi-dev
```
4. Compile
```
cd code
./bootstrap
./configure --enable-maintainer-mode --disable-werror --enable-buspirate
make
sudo make install
```
5. Connect the BusPirate to Rpi as follows

| BusPirate     | Rpi           |
| ------------- |-------------:|
| MISO      | right-aligned |
| MOSI      | centered      |
| CS |       |
|CLK|	|
|Aux|	|

6. Then use the openocd.cfg file
```
sudo openocd -f openocd.cfg
```
