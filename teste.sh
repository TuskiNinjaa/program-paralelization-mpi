#!/bin/bash

gcc histograma_seq.c -o histograma_seq -Wall

for i in {1..4}
do
	./histograma_seq ./Arquivos/e$i.txt ./Arquivos/sa$i.txt
    cmp --silent ./Arquivos/s$i.txt ./Arquivos/sa$i.txt && echo "Test case $i: Files are EQUAL" || echo "Test case $i: Files are DIFFERENT"
done