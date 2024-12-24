#!/bin/sh

. ./config.sh

cd $KERNEL_DIR
make clean
make 
cd ..

cd $LIBC_DIR
make clean
make
cd ../../

mkdir -p $BUILD_DIR
cp $KERNEL_DIR/$KERNEL_BIN $BUILD_DIR/$KERNEL_BIN