project(Multithread)

cmake_minimum_required(VERSION "3.22.1")

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(${PROJECT_SOURCE_DIR}/include)
add_subdirectory(${PROJECT_SOURCE_DIR}/src)
add_subdirectory(${PROJECT_SOURCE_DIR}/test)


enable_testing()
add_test(NAME test_countdown_latch COMMAND test_countdown_latch)
