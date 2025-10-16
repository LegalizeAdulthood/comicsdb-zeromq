cmake_minimum_required(VERSION 3.22)
set(COMICSDB_PROJECT_NAME "comicsdb" CACHE STRING "Project name")
project(${COMICSDB_PROJECT_NAME})

find_package(RapidJSON CONFIG REQUIRED)
find_package(ZeroMQ CONFIG REQUIRED)
find_package(cppzmq CONFIG REQUIRED)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(comicsdb)
