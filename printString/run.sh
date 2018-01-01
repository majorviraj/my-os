#!/bin/bash

make clean
make all
rm -rf /media/chinmay/boot1/kernel.img
cp kernel.img /media/chinmay/boot1/kernel.img
umount /media/chinmay/boot
