#!/bin/bash
rm payload.elf payload.bin;
arm-none-eabi-gcc payload.c -fpie -W -pedantic -Wno-long-long -Os -L. -nostdlib -emain -march=armv7-a -mthumb -o payload.elf;
arm-none-eabi-objcopy -O binary payload.elf payload.bin; rm payload.elf
HASH=`openssl sha1 -binary payload.bin | xxd -p`
printf "SHA1: $HASH\n";
printf "DATA: ";
printf %s `xxd -p payload.bin`
printf "\n"
