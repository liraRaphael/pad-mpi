cd ../src/
make
cd ../bin/
bash copy.sh
mpirun -n 8 -hostfile ../hosts programa 1000 1000 1000 arqA_1000.dat arqB_1000.dat arqC_1000.dat arqD1000.dat
