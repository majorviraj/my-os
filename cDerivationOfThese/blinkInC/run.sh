#!/bin/bash

boot=1A8D-4341
udisksctl mount -b /dev/mmcblk0p1 &&
make all &&
rm -rf /media/chinmay/$boot/kernel.img &&
cp -v kernel.img /media/chinmay/$boot/kernel.img 
udisksctl unmount -b /dev/mmcblk0p1
