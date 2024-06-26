#!/bin/bash
make
qemu-system-x86_64 -hda disk.img
