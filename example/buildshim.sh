#!/bin/bash
rm shim.elf shim.bin; 
arm-none-eabi-gcc shim.c -fpie -W -pedantic -Wno-long-long -Os -L. -nostdlib -ebdev_read -march=armv7-a -mthumb -o shim.elf; 
arm-none-eabi-objcopy -O binary shim.elf shim.bin; rm shim.elf
HASH=`openssl sha1 -binary shim.bin | xxd -p`
printf "SHA1: $HASH\n";
printf "DATA: ";
printf %s `xxd -p shim.bin`
printf "\n"
