#!/bin/bash
#========== Slurm Options ==========
#SBATCH -p gr20100b
#SBATCH -t 1:00:00
#SBATCH --rsc p=1:t=112:c=112:m=500G
#SBATCH -o %x.%j.out

#========== Shell Script ==========
set -x

srun ./cpu_test