#!/bin/bash
#SBATCH -p gr20100b
#SBATCH --rsc-list p=1:t=112:c=112:m=500G

srun ./cpu_test