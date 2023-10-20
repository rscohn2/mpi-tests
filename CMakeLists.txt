cmake_minimum_required(VERSION 3.20)
project(
  mpi-tests
  VERSION 0.1
  DESCRIPTION "MPI Tests")

include(CTest)

set(CMAKE_CXX_STANDARD 20)

function(add_mpi_test test_name name processes)
  add_test(NAME ${test_name}
           COMMAND ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} ${processes}
                   ${MPIEXEC_PREFLAGS} ./${name} ${ARGN} COMMAND_EXPAND_LISTS)
endfunction()

add_compile_options(-fsycl)
add_link_options(-fsycl)
find_package(MPI REQUIRED)
link_libraries(MPI::MPI_CXX)

add_executable(send-receive send-receive.cpp)
add_mpi_test(send-receive-device-2 send-receive 2 device)
add_mpi_test(send-receive-shared-2 send-receive 2)

add_executable(put-get put-get.cpp)
add_mpi_test(put-get-device-2 put-get 2 device)
add_mpi_test(put-get-shared-2 put-get 2)
