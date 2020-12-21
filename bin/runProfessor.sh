cd ../src/
make
cd ../bin/
bash copy.sh
mpirun -n 8 -hostfile ../hosts programa 997 981 991 arqA_997x981.dat arqB_981x991.dat arqC_991x1.dat arqD.dat
