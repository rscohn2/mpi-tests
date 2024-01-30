#include <sycl/sycl.hpp>

#include "mpi.h"

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  MPI_Comm comm = MPI_COMM_WORLD;
  int rank, size;
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);

  std::size_t sz = 1000;
  sycl::queue q;
  int *data = nullptr;
  bool use_device = argc >= 2 && std::string(argv[1]) == "device";
  bool use_shared = argc >= 2 && std::string(argv[1]) == "shared";
  if (use_device) {
    std::cout << "Allocating device memory\n";
    data = sycl::malloc_device<int>(sz, q);
  } else if (use_shared) {
    std::cout << "Allocating shared memory\n";
    data = sycl::malloc_shared<int>(sz, q);
  } else {
    std::cout << "Allocating system memory\n";
    data = static_cast<int*>(malloc(sz));
  }

  MPI_Win win;
  MPI_Win_create(data, sz, 1, MPI_INFO_NULL, comm, &win);
  MPI_Win_fence(0, win);

  
  int dst_rank = (rank + 1) % size;
  int src_rank = (rank - 1 + size) % size;

  int put_val = 1;
  MPI_Put(&put_val, 1, MPI_INT, dst_rank, 0, 1, MPI_INT, win);
  MPI_Win_fence(0, win);
  int put_res = 0;
  q.copy(data, &put_res, 1).wait();
  assert(put_res == put_val);

  if (use_device || use_shared) {
    sycl::free(data, q);
  } else {
    free(data);
  }
  MPI_Finalize();
  return 0;
}
