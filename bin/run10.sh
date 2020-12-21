cd ../src/
make
cd ../bin/
bash copy.sh
mpirun -n 8 -hostfile ../hosts programa 10 10 10 arqA_10.dat arqB_10.dat arqC_10.dat arqD10.dat