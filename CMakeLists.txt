cmake_minimum_required(VERSION 3.22)
project(comicsdb)

find_package(RapidJSON CONFIG REQUIRED)
find_package(ZeroMQ CONFIG REQUIRED)
find_package(cppzmq CONFIG REQUIRED)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(comicsdb)
