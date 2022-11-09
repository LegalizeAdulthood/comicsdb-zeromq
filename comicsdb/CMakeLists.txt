# Application logic
add_library(comicsdb
  comicsdb.h
  comicsdb.cpp
  comic.h
  comic.cpp
  json.h
  json.cpp
)
target_link_libraries(comicsdb PUBLIC rapidjson)

add_executable(zmqClient zmqClient.cpp)
target_link_libraries(zmqClient PRIVATE cppzmq libzmq)

add_executable(zmqService zmqService.cpp)
target_link_libraries(zmqService PRIVATE cppzmq libzmq)
