cmake_minimum_required(VERSION 3.22)
project(next CXX)

set(CMAKE_CXX_STANDARD 17)
add_link_options(-fsanitize=undefined)
add_compile_options(-pedantic -Wvla -Werror -fstack-protector-strong
        -fsanitize=undefined -fno-sanitize-recover -g -fno-omit-frame-pointer -O0)

add_executable(next next.cpp next.h main.cpp)
