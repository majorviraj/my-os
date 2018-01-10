#!/bin/bash

udisksctl mount -b /dev/mmcblk0p1 &&
make all &&
rm -rf /media/chinmay/boot/kernel.img &&
cp kernel.img /media/chinmay/boot1/kernel.img 
udisksctl unmount -b /dev/mmcblk0p1