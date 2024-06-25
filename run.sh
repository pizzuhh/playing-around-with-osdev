#!/bin/bash
make
qemu-system-i386 -hda disk.img
