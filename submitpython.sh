#!/bin/bash -l

#SBATCH --job-name=PerfEval
#SBATCH --time=0-0:25:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=16
#SBATCH --mem=16G
#SBATCH --constraint=moles

module load Python/3.10.4-GCCcore-11.3.0

source ~/virtualenvs/test/bin/activate

python original.py

