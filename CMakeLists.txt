cmake_minimum_required(VERSION 3.10)
project(IDCT_Sim CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB RTL_SOURCES "${CMAKE_SOURCE_DIR}/rtl/*.v" "${CMAKE_SOURCE_DIR}/rtl/*.sv")
file(GLOB TB_SOURCES_CPP "${CMAKE_SOURCE_DIR}/tb/src/*.cpp" "${CMAKE_SOURCE_DIR}/tb/main.cpp")

find_program(VERILATOR_EXECUTABLE verilator)
if(NOT VERILATOR_EXECUTABLE)
  message(FATAL_ERROR "Verilator not found!")
endif()

set(TOP_MODULE "int_dct8")
set(VERILATOR_CFLAGS "-I${CMAKE_SOURCE_DIR}/tb/include -I${CMAKE_BINARY_DIR}/obj_dir -std=c++20")

add_custom_target(build_verilator ALL
  COMMAND ${VERILATOR_EXECUTABLE} --cc --exe --build --trace --top-module ${TOP_MODULE} -CFLAGS "${VERILATOR_CFLAGS}" ${RTL_SOURCES} ${TB_SOURCES_CPP}
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(run_sim
  COMMAND ${CMAKE_BINARY_DIR}/obj_dir/V${TOP_MODULE}
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  DEPENDS build_verilator
)
