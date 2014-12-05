#!/bin/sh
make clean
make
clear
./testVec
/opt/valgrind-3.7.0/bin/valgrind --tool=memcheck ./testVec
