cd ../src/
make
cd ../bin/
bash copy.sh
mpirun -n 8 -hostfile ../hosts programa 100 100 100 arqA_100.dat arqB_100.dat arqC_100.dat arqD100.dat
