cmake_minimum_required(VERSION 4.0.0)

project(p2996_tryouts CXX)

include(CheckCXXSourceCompiles)

check_cxx_source_compiles(
  [=[
#include <meta>
int main() {}
]=]
  RESULT)

if(NOT RESULT)
  message(FATAL_ERROR "No reflection for you")
endif()

add_executable(out main.cpp)
target_compile_features(out PRIVATE cxx_std_26)
