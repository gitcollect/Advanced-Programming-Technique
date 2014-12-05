#!/bin/tcsh
#PBS -q class
#PBS -l nodes=1:sixcore
#PBS -l walltime=00:10:00
#PBS -N QichaoChu
# The below chnages the working directory to the location of
# your threaded DFT program
cd ThreadsTransform2D
make
rm Qichao*
rm my1d.txt
rm my2d.txt
rm myiv.txt
touch my1d.txt
touch my2d.txt
touch myiv.txt
./threadDFT2d



