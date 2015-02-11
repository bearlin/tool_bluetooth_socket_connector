#!/bin/bash

#mkdir -p Build && cd Build && rm -rf && cmake .. && make && cd -
#mkdir -p Build && cd Build && rm -rf && cmake -DCMAKE_BUILD_TYPE=Debug -Dthirdparty_build_gmock=ON .. && make && cd -
mkdir -p Build && cd Build && rm -rf && cmake -DCMAKE_BUILD_TYPE=Debug .. && make && cd -
