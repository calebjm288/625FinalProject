#!/bin/bash -l

#SBATCH --job-name=ParaMemo
#SBATCH --time=0-0:25:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --mem=30G
#SBATCH --constraint=moles

module load foss

gcc -o Triangle_Memo Triangle_Memoization.c -lm
if [ $? -eq 0 ]; then
  echo "Compilation successful."
  # Run the simulation 10 times to get more data
  for ((i=1; i<=3; i++)); do
    ./Triangle_Memo inputfile500.dat
  done
  for ((i=1; i<=3; i++)); do
    ./Triangle_Memo inputfile1000.dat
  done
  for ((i=1; i<=3; i++)); do
    ./Triangle_Memo inputfile2000.dat
  done
else
  echo "Compilation failed."
fi

#module load Python/3.10.4-GCCcore-11.3.0

#source ~/virtualenvs/test/bin/activate

#python Parallel_Memoization.py
#python Triangle_Memoization.py

