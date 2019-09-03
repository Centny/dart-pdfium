#!/bin/bash
set -e
rm -rf out
mkdir out
cd out
cmake .. -DCMAKE_INSTALL_PREFIX=../ -DCODE_COVERAGE=ON
make
make install
cd ../

pub run test_coverage test/

llvm-profdata merge -sparse default.profraw -o default.profdata
llvm-cov show lib/libdart_pdfium.dylib -instr-profile=default.profdata src/*.cc -path-equivalence -use-color --format html > coverage/coverage.html
genhtml -o coverage coverage/lcov.info

#https://storage.googleapis.com/dart-archive/channels/stable/release/latest/sdk/dartsdk-macos-x64-release.zip