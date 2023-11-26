cmake_minimum_required(VERSION 3.17)

set(CMAKE_C_STANDART 99)
set(CMAKE_C_STANDART_REQUIRED ON)

set(PROJECT_NAME mygrep) 
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} main.c)
