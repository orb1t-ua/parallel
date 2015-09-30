#!/bin/bash
#PBS -l nodes=1:ppn=10
#PBS -l walltime=00:15:00
#PBS -N tdhughes44_pthread_pi
#PBS -m bea
#PBS -M tdhughes44@students.tntech.edu
cd ~/pi
./run.sh 10

