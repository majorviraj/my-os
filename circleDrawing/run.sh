#!/bin/bash

make -j4 all &&
rm -rf /media/chinmay/boot/kernel.img &&
udisksctl mount -b /dev/mmcblk0p1 &&
cp kernel.img /media/chinmay/boot1/kernel.img
udisksctl unmount -b /dev/mmcblk0p1
