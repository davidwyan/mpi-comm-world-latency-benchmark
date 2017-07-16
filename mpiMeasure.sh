#!/bin/bash
numProcesses=$1
numIterations=$2
dataSize=$3
mpicc -O3 mpiTool.c
mpirun --np $numProcesses --default-hostfile mpd.hosts ./a.out $numIterations $dataSize
