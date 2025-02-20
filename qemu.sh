#!/bin/sh

. ./iso.sh

qemu-system-i386 -cdrom $ISODIR/supdiff.iso -d int