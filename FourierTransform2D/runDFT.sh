#!/bin/tcsh
#PBS -q class
#PBS -l nodes=2:sixcore
#PBS -l walltime=00:10:00
#PBS -N Qichao
# The below chnages the working directory to the location of
# your testmpi program
cd /nethome/qchu3/FourierTransform2D
rm Qichao*
# The below tells MPI to run your jobs on 16 processors
/usr/lib64/openmpi/bin/mpirun -np 16 --hostfile $PBS_NODEFILE --prefix /usr/lib64/openmpi/ ./fft2d
