#include <sycl/sycl.hpp>

int main(int argc, char *argv[]) {
  auto devices = sycl::platform().get_devices();

  assert(devices.size() > 1);
  
  std::cout << "Device 0: " << devices[0].get_info<sycl::info::device::name>() << "\n";
  std::cout << "Device 1: " << devices[1].get_info<sycl::info::device::name>() << "\n";
  auto q0 = sycl::queue(devices[0]);
  auto q1 = sycl::queue(devices[0]);

  std::size_t sz = 100;
  auto m0 = sycl::malloc_device<int>(sz, q0);
  auto m1 = sycl::malloc_device<int>(sz, q1);
  q0.memcpy(m0, m1, sizeof(int) * sz).wait();
  q1.memcpy(m0, m1, sizeof(int) * sz).wait();
  q0.single_task([=]() {
    *m0 = *m1;
  }).wait();
  q1.single_task([=]() {
    *m0 = *m1;
  }).wait();
  return 0;
}
