#!/bin/bash

gcc histograma_seq.c -o histograma_seq -Wall
mpicc histograma.c -o histograma -Wall

for i in {1..4}
do
	./histograma_seq ./Arquivos/e$i.txt ./Arquivos/s$i.txt
	# mpirun -use-hwthread-cpus histograma ./Arquivos/e$i.txt ./Arquivos/sp$i.txt
	mpirun -np 7 histograma ./Arquivos/e$i.txt ./Arquivos/sp$i.txt
    cmp --silent ./Arquivos/s$i.txt ./Arquivos/sp$i.txt && echo "Test case $i: Files are EQUAL" || echo "Test case $i: Files are DIFFERENT"
done