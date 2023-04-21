#!/bin/bash

gnome-terminal -- /opt/spike/bin/spike -H --isa=RV32IM --rbb-port=9824 -m0x40000000:0x01000000,0xFFFF0000:0x10000 ./Debug/icusw
gnome-terminal -- /opt/openocd/bin/openocd -f spike.cfg
