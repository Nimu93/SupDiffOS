#!/bin/sh

. ./build.sh

mkdir -p $ISODIR
mkdir -p $ISODIR/boot
mkdir -p $ISODIR/boot/grub

cp $BUILD_DIR/$KERNEL_BIN $ISODIR/boot/$KERNEL_BIN

cat > $ISODIR/boot/grub/grub.cfg << EOF
menuentry "supdiff" {
    multiboot /boot/$KERNEL_BIN
}
EOF

$GRUB_MKRESCUE -o $ISODIR/supdiff.iso $ISODIR


