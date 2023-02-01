add_executable(test_countdown_latch
    ${PROJECT_SOURCE_DIR}/src/countdown_latch.cc
    ${PROJECT_SOURCE_DIR}/test/test_countdown_latch.cc)

target_include_directories(test_countdown_latch PUBLIC ${PROJECT_SOURCE_DIR}/include)
