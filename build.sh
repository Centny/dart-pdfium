#!/bin/bash
rm -rf out
mkdir out
cd out
cmake .. -DCMAKE_INSTALL_PREFIX=../
make && make install
cd ../

pub run test_coverage test/
genhtml -o coverage coverage/lcov.info

#https://storage.googleapis.com/dart-archive/channels/stable/release/latest/sdk/dartsdk-macos-x64-release.zip