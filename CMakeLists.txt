cmake_minimum_required(VERSION 3.22)
set(UNIT_NAME Reactor)
project(${UNIT_NAME}Project)

include(CTest)


add_subdirectory(src)
add_subdirectory(test)
