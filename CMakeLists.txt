cmake_minimum_required(VERSION 4.0.0)

project(reflection_tryouts CXX)

include(CheckCXXSourceCompiles)

check_cxx_source_compiles(
  [=[
#include <meta>
int main() {}
]=]
  RESULT)

if(NOT RESULT)
  message(FATAL_ERROR "No support for reflection on your compiler")
endif()

# https://wg21.link/P2996
add_subdirectory(examples_on_proposals/P2996)

# https://wg21.link/P3096
add_subdirectory(examples_on_proposals/P3096)

# https://wg21.link/P3491
# add_subdirectory(examples_on_proposals/P3491)
