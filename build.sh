#!/bin/sh

. ./config.sh

for PROJECT in $PROJECTS; do
    (cd $PROJECT && make )
done

mkdir -p $BUILD_DIR
cp $KERNEL_DIR/$KERNEL_BIN $BUILD_DIR/$KERNEL_BIN