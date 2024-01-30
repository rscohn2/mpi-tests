#include <sycl/sycl.hpp>

#include "mpi.h"

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  MPI_Comm comm = MPI_COMM_WORLD;
  int rank, size;
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);
  std::cout << "Rank: " << rank << " size: " << size << "\n";

  sycl::queue q;
  int *data = nullptr;
  bool use_device = argc >= 2 && std::string(argv[1]) == "device";
  bool use_shared = argc >= 2 && std::string(argv[1]) == "shared";
  if (use_device) {
    std::cout << "Allocating device memory\n";
    data = sycl::malloc_device<int>(1, q);
  } else if (use_shared) {
    std::cout << "Allocating shared memory\n";
    data = sycl::malloc_shared<int>(1, q);
  } else {
    std::cout << "Allocating system memory\n";
    data = static_cast<int*>(malloc(1));
  }
  
  MPI_Request send_request, recv_request;
  int dst_rank = (rank + 1) % size;
  int src_rank = (rank - 1 + size) % size;
  std::cout << "rank: " << rank << " dest: " << dst_rank << " src: " << src_rank << "\n";
  MPI_Irecv(data, 1, MPI_INT, src_rank, 0, comm, &recv_request);
  std::cout << "initiated receive\n";
  MPI_Isend(data, 1, MPI_INT, dst_rank, 0, comm, &send_request);
  std::cout << "initiated send\n";

  MPI_Wait(&send_request, MPI_STATUS_IGNORE);
  std::cout << "Send complete\n";
  MPI_Wait(&recv_request, MPI_STATUS_IGNORE);
  std::cout << "Receive complete\n";
  
  if (use_shared || use_device) {
    sycl::free(data, q);
  } else {
    free(data);
  }
  MPI_Finalize();
  return 0;
}
