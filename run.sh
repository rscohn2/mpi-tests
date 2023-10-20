source /opt/intel/oneapi/setvars.sh
mkdir build
cd build
cmake ..
make -j
I_MPI_OFFLOAD=0 mpirun -n 2  ./send-receive
I_MPI_OFFLOAD=0 mpirun -n 2  ./put-get
I_MPI_OFFLOAD=1 mpirun -n 2  ./send-receive device
I_MPI_OFFLOAD=1 mpirun -n 2  ./put-get device
