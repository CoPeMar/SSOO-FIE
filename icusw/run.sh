#!/bin/bash

/opt/spike/bin/spike --isa=RV32IM  --real-time-clint -m0x40000000:0x01000000,0xFFFF0000:0x10000 ./Debug/icusw