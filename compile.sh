#!/bin/bash


dart(){
    cd pdfium
    mkdir -p out
    cp -f ../pdfium-dart.args.gn out/args.gn
    gn gen out/
    ninja -C out/ pdfium
    cd ../

    rm -rf out/dart/
    mkdir -p out/dart/
    cd out/dart/
    cmake ../.. -DCMAKE_INSTALL_PREFIX=../../
    make
    make install
    cd ../../
}

flutter(){
    rm -rf out/flutter/
    mkdir -p out/flutter/
    out_dir=`pwd`/out/flutter/

    cd pdfium
    mkdir -p out_mac_x64 out_ios_x64 out_ios_arm64

    ## for mac x64
    cp -f ../pdfium-mac.args.gn out_mac_x64/args.gn
    gn gen out_mac_x64/
    ninja -C out_mac_x64/ pdfium
    mkdir -p $out_dir/mac_x64/
    cp -f out_mac_x64/lib*.dylib $out_dir/mac_x64/

    ## for ios x64
    cp -f ../pdfium-mac.args.gn out_ios_x64/args.gn
    echo 'target_os="ios"' >>out_ios_x64/args.gn
    echo 'target_cpu="x64"' >>out_ios_x64/args.gn
    gn gen out_ios_x64/
    ninja -C out_ios_x64/ pdfium
    mkdir -p $out_dir/ios_x64/
    cp -f out_ios_x64/lib*.dylib $out_dir/ios_x64/

    ## for ios arm64
    cp -f ../pdfium-mac.args.gn out_ios_arm64/args.gn
    echo 'target_os="ios"' >>out_ios_arm64/args.gn
    echo 'target_cpu="arm64"' >>out_ios_arm64/args.gn
    gn gen out_ios_arm64/
    ninja -C out_ios_arm64/ pdfium
    mkdir -p $out_dir/ios_arm64/
    cp -f out_ios_arm64/lib*.dylib $out_dir/ios_arm64/

    ## for lipo
    mkdir -p $out_dir/combined
    lipo \
        $out_dir/ios_x64/libpdfium.dylib \
        $out_dir/ios_arm64/libpdfium.dylib \
        -output  $out_dir/combined/libpdfium.dylib -create
    lipo \
        $out_dir/ios_x64/libicuuc.dylib \
        $out_dir/ios_arm64/libicuuc.dylib \
        -output  $out_dir/combined/libicuuc.dylib -create
    lipo \
        $out_dir/ios_x64/libchrome_zlib.dylib \
        $out_dir/ios_arm64/libchrome_zlib.dylib \
        -output  $out_dir/combined/libchrome_zlib.dylib -create 
    ##
    cd ../
    cp -f scripts/xcode_backend.sh $out_dir/
    cp -rf pdfium/public $out_dir/
}

case $1 in
flutter)
    flutter
;;
dart)
    dart
;;
esac

