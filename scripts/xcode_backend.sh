#!/bin/bash
wdir=`dirname ${0}`
case $ARCHS in
x86_64)
    echo "copy x64 library to $PROJECT_DIR/Flutter/"
    cp -f $wdir/ios_x64/lib*.dylib $PROJECT_DIR/Flutter/
    exit 0
;;
arm64)
    echo "copy arm64 library to $PROJECT_DIR/Flutter/"
    cp -f $wdir/ios_arm64/lib*.dylib $PROJECT_DIR/Flutter/
    exit 0
;;
*)
    echo not supported arch $ARCHS
    exit 1
;;
esac