cmake_minimum_required(VERSION 3.23)
project(arytmetyka C)

set(CMAKE_C_STANDARD 17)
add_link_options(-fsanitize=undefined)
add_compile_options(-pedantic -Wall -Wextra -Wformat-security -Wduplicated-cond -Wfloat-equal -Wshadow -Wconversion
        -Wjump-misses-init -Wlogical-not-parentheses -Wnull-dereference -Wvla -Werror -fstack-protector-strong
        -fsanitize=undefined -fno-sanitize-recover -g -fno-omit-frame-pointer -O1)

add_executable(arytmetyka main.c ary.c)
