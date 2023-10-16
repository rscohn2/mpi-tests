#include <sycl/sycl.hpp>

#include "mpi.h"

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  MPI_Comm comm = MPI_COMM_WORLD;
  int rank, size;
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);

  sycl::queue q;
  int *data = nullptr;
  bool use_device = argc >= 2 && std::string(argv[1]) == "device";
  if (use_device) {
    std::cout << "Allocating device memory\n";
    data = sycl::malloc_device<int>(1, q);
  } else {
    std::cout << "Allocating shared memory\n";
    data = sycl::malloc_shared<int>(1, q);
  }
  
  MPI_Request send_request, recv_request;
  MPI_Isend(data, 1, MPI_INT, (rank + 1) % size , 0, comm, &send_request);
  MPI_Irecv(data, 1, MPI_INT, (rank - 1) % size, 0, comm, &recv_request);
  MPI_Wait(&send_request, MPI_STATUS_IGNORE);
  std::cout << "Send complete\n";
  MPI_Wait(&recv_request, MPI_STATUS_IGNORE);
  std::cout << "Receive complete\n";
  
  sycl::free(data, q);
  MPI_Finalize();
  return 0;
}
