#!/bin/bash

make clean
make all
rm -rf /media/chinmay/boot/kernel.img
cp kernel.img /media/chinmay/boot/kernel.img
umount /media/chinmay/boot