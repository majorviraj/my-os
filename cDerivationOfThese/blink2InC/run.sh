#!/bin/bash

boot=2DBD-A7B6
udisksctl mount -b /dev/sdb1 &&
make all &&
rm -rf /media/chinmay/$boot/kernel.img &&
cp -v kernel.img /media/chinmay/$boot/kernel.img 
udisksctl unmount -b /dev/sdb1
