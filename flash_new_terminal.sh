#!/bin/bash

export IDF_PATH="/home/dinux/esp/esp-idf"; . "$IDF_PATH/export.sh"

esptool.py --port /dev/ttyUSB0 write_flash -z 0x1000 /mnt/5b4e6c46-c675-42a8-9393-64ca549dee63/microcontrollers/new_board/acan3_S3/build/bootloader/bootloader.bin


idf.py flash-bootloder