#!/bin/bash
make all
<<<<<<< HEAD
rm -rf /media/chinmay/boot1/kernel.img
cp kernel.img /media/chinmay/boot1/kernel.img
umount /media/chinmay/boot
=======
cp kernel.img /media/majorviraj/boot
umount /dev/mmcblk0p1
umount /dev/mmcblk0p2
>>>>>>> 1443e47b8460506c3efa164c0d2c98985812a826
