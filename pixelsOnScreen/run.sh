#!/bin/bash
make all
cp kernel.img /media/majorviraj/boot
umount /dev/mmcblk0p1
umount /dev/mmcblk0p2