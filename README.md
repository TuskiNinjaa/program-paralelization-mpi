# program-paralelization-mpi
The objective of this work is to use the MPI library to parallelize a sequential algorithm (Histogram Calculation) and verify the speedup obtained when executing both codes on the same machine.

# Setup conda enviroment
```console
conda env create -f pp_mpi.yml
```

# Unzip inputs and outputs files
```console
unzip Arquivos.zip
```

# Test script
```console
chmod +x teste.sh
```

```console
./teste.sh
```

## Example of output after running the test script

```console
[all threads]
Tempo=0.00 segundos
Tempo=0.00 segundos
Test case 1: Files are EQUAL
Tempo=0.00 segundos
Tempo=0.00 segundos
Test case 2: Files are EQUAL
Tempo=0.00 segundos
Tempo=0.00 segundos
Test case 3: Files are EQUAL
Tempo=0.24 segundos
Tempo=0.20 segundos
Test case 4: Files are EQUAL
[1 threads]
Tempo=0.00 segundos
Tempo=0.00 segundos
Test case 1: Files are EQUAL
Tempo=0.00 segundos
Tempo=0.00 segundos
Test case 2: Files are EQUAL
Tempo=0.00 segundos
Tempo=0.00 segundos
Test case 3: Files are EQUAL
Tempo=0.25 segundos
Tempo=0.24 segundos
Test case 4: Files are EQUAL
[2 threads]
Tempo=0.00 segundos
Tempo=0.00 segundos
Test case 1: Files are EQUAL
Tempo=0.00 segundos
Tempo=0.00 segundos
Test case 2: Files are EQUAL
Tempo=0.00 segundos
Tempo=0.00 segundos
Test case 3: Files are EQUAL
Tempo=0.24 segundos
Tempo=0.18 segundos
Test case 4: Files are EQUAL
[3 threads]
Tempo=0.00 segundos
Tempo=0.00 segundos
Test case 1: Files are EQUAL
Tempo=0.00 segundos
Tempo=0.00 segundos
Test case 2: Files are EQUAL
Tempo=0.00 segundos
Tempo=0.00 segundos
Test case 3: Files are EQUAL
Tempo=0.25 segundos
Tempo=0.17 segundos
Test case 4: Files are EQUAL
[4 threads]
Tempo=0.00 segundos
Tempo=0.00 segundos
Test case 1: Files are EQUAL
Tempo=0.00 segundos
Tempo=0.00 segundos
Test case 2: Files are EQUAL
Tempo=0.00 segundos
Tempo=0.00 segundos
Test case 3: Files are EQUAL
Tempo=0.24 segundos
Tempo=0.15 segundos
Test case 4: Files are EQUAL
[5 threads]
Tempo=0.00 segundos
Tempo=0.00 segundos
Test case 1: Files are EQUAL
Tempo=0.00 segundos
Tempo=0.00 segundos
Test case 2: Files are EQUAL
Tempo=0.00 segundos
Tempo=0.00 segundos
Test case 3: Files are EQUAL
Tempo=0.25 segundos
Tempo=0.15 segundos
Test case 4: Files are EQUAL
[6 threads]
Tempo=0.00 segundos
Tempo=0.00 segundos
Test case 1: Files are EQUAL
Tempo=0.00 segundos
Tempo=0.00 segundos
Test case 2: Files are EQUAL
Tempo=0.00 segundos
Tempo=0.00 segundos
Test case 3: Files are EQUAL
Tempo=0.25 segundos
Tempo=0.15 segundos
Test case 4: Files are EQUAL
[7 threads]
Tempo=0.00 segundos
Tempo=0.00 segundos
Test case 1: Files are EQUAL
Tempo=0.00 segundos
Tempo=0.00 segundos
Test case 2: Files are EQUAL
Tempo=0.00 segundos
Tempo=0.00 segundos
Test case 3: Files are EQUAL
Tempo=0.24 segundos
Tempo=0.19 segundos
Test case 4: Files are EQUAL
[8 threads]
Tempo=0.00 segundos
Tempo=0.00 segundos
Test case 1: Files are EQUAL
Tempo=0.00 segundos
Tempo=0.00 segundos
Test case 2: Files are EQUAL
Tempo=0.00 segundos
Tempo=0.00 segundos
Test case 3: Files are EQUAL
Tempo=0.24 segundos
Tempo=0.19 segundos
Test case 4: Files are EQUAL
```