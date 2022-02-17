cmake_minimum_required(VERSION 3.20)
project(hw_1)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)

add_executable(hw_1 src/main.c src/bmp.c include/bmp.h src/stego.c include/stego.h)
