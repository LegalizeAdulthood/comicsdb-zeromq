# Application logic
add_library(comicsdb
  comicsdb.h
  comicsdb.cpp
  comic.h
  comic.cpp
  json.h
  json.cpp
)
target_include_directories(comicsdb PUBLIC .)
target_link_libraries(comicsdb PUBLIC rapidjson)

add_executable(zmqClient zmqClient.cpp)
target_link_libraries(zmqClient PRIVATE cppzmq libzmq)

add_executable(zmqService zmqService.cpp)
target_link_libraries(zmqService PRIVATE cppzmq libzmq)

add_executable(comicsClient comicsClient.cpp)
target_link_libraries(comicsClient comicsdb cppzmq libzmq rapidjson)

add_executable(comicsService comicsService.cpp)
target_link_libraries(comicsService comicsdb cppzmq libzmq rapidjson)
