cmake_minimum_required(VERSION 3.20)
project(
  mpi-tests
  VERSION 0.1
  DESCRIPTION "MPI Tests")

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-fsycl)
add_link_options(-fsycl)
find_package(MPI REQUIRED)
link_libraries(MPI::MPI_CXX)

add_executable(send-device send-device.cpp)
add_executable(send-shared send-shared.cpp)
