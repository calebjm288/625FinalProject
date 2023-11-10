#!/bin/bash -l

#SBATCH --job-name=PerfEval
#SBATCH --time=0-0:20:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=16
#SBATCH --mem=8G
#SBATCH --constraint=moles

module load foss
#module load foss GSL/2.6-GCC-9.3.0

mpicc -o mpi mpi.c -lm

mpirun mpi
