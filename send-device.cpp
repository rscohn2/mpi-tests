#include <sycl/sycl.hpp>

#include "mpi.h"

int main(int argc, char *argv[]) {
  sycl::queue q;
  auto data = sycl::malloc_device<int>(1, q);
  
  MPI_Init(&argc, &argv);

  
  MPI_Request send_request, recv_request;
  MPI_Isend(data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &send_request);
  MPI_Irecv(data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &recv_request);
  MPI_Wait(&send_request, MPI_STATUS_IGNORE);
  MPI_Wait(&recv_request, MPI_STATUS_IGNORE);

  sycl::free(data, q);
  MPI_Finalize();
  return 0;
}
