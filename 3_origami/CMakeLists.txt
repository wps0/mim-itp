cmake_minimum_required(VERSION 3.23)
project(origami C CXX)

set(CMAKE_CXX_STANDARD 17)
add_link_options(-fsanitize=undefined)
add_compile_options(-pedantic -Wvla -Werror -fstack-protector-strong
        -fsanitize=undefined -fno-sanitize-recover -g -fno-omit-frame-pointer -O1)

add_executable(origami origami.c)

add_executable(wizualizator wizualizator.cpp)
