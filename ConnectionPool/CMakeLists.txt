project(ThreadPool)

cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(${PROJECT_SOURCE_DIR}/include)
add_subdirectory(${PROJECT_SOURCE_DIR}/src)
add_subdirectory(${PROJECT_SOURCE_DIR}/test)

enable_testing()
add_test(NAME test_mysql_connection COMMAND test_mysql_connection)
add_test(NAME test_connection_pool COMMAND test_connection_pool)
