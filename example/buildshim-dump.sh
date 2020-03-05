#!/bin/bash
rm shim-dump.elf shim-dump.bin; 
arm-none-eabi-gcc shim-dump.c -fpie -W -pedantic -Wno-long-long -Os -L. -nostdlib -ebdev_read -march=armv7-a -mthumb -o shim-dump.elf; 
arm-none-eabi-objcopy -O binary shim-dump.elf shim-dump.bin; rm shim-dump.elf
HASH=`openssl sha1 -binary shim-dump.bin | xxd -p`
printf "SHA1: $HASH\n";
printf "DATA: ";
printf %s `xxd -p shim-dump.bin`
printf "\n"
