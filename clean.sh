. ./config.sh

for PROJECT in $PROJECTS; do
    (cd $PROJECT && make clean)
done

rm -rf $BUILD_DIR
rm -rf $ISODIR
rm -f $KERNEL_DIR/*.o
rm -f $KERNEL_DIR/*.bin