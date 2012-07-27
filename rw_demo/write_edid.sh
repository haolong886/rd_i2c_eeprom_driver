#!/bin/sh
EEPROM=/sys/eeprom/eeprom_link/eeprom_ctl
echo "$EEPROM"
echo "00,00,00" > /sys/eeprom/eeprom_link/eeprom_ctl
sleep 0.1
echo "00,01,aa" > /sys/eeprom/eeprom_link/eeprom_ctl
echo "00,02,ff" > /sys/eeprom/eeprom_link/eeprom_ctl
echo "00,03,ff" > $EEPROM
echo "00,04,ff" > $EEPROM
echo "00,05,ff" > $EEPROM
echo "00,06,ff" > $EEPROM
echo "00,07,00" > $EEPROM
#echo "00,00,00" > /mnt/mmcp2/tmp/haolong/aa
#echo "ff,ff,ab" > /mnt/mmcp2/tmp/haolong/aa
