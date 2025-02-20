#!/bin/sh

. ./iso.sh

qemu-system-i386 -d int -cdrom $ISODIR/supdiff.iso -s -S