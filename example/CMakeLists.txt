cmake_minimum_required(VERSION 3.16)

set(JSON_LIBS TinyJson)

add_executable(test_all test_all.cpp)
target_link_libraries(test_all ${JSON_LIBS})

add_executable(parse_stringify parse_stringify.cpp)
target_link_libraries(parse_stringify ${JSON_LIBS})

add_executable(filestream filestream.cpp)
target_link_libraries(filestream ${JSON_LIBS})