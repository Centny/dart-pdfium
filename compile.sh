#!/bin/bash


dart(){
    cd pdfium
    mkdir -p out
    cp -f ../pdfium-dart.args.gn out/args.gn
    gn gen out/
    ninja -C out/ pdfium
    cd ../

    rm -rf out
    mkdir -p out
    cd out
    cmake .. -DCMAKE_INSTALL_PREFIX=../
    make
    make install
    cd ../
}

flutter(){
    rm -rf out
    mkdir -p out
    out_dir=`pwd`/out/

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
}

case $1 in
flutter)
    flutter
;;
dart)
    dart
;;
esac

