#!/bin/bash
make
qemu-system-x86_64 -drive file=disk.img,format=raw
