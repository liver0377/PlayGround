add_executable(
  test_mysql_connection test_mysql_connection.cc
                        ${PROJECT_SOURCE_DIR}/src/mysql_connection.cc)
target_include_directories(test_mysql_connection
                           PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(test_mysql_connection mysqlclient)
target_compile_options(test_mysql_connection PUBLIC "-g")

add_executable(
  test_connection_pool
  test_connection_pool.cc ${PROJECT_SOURCE_DIR}/src/connection_pool.cc
  ${PROJECT_SOURCE_DIR}/src/mysql_connection.cc)
target_include_directories(
  test_connection_pool
  PUBLIC ${PROJECT_SOURCE_DIR}
  PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(test_connection_pool mysqlclient pthread)
target_compile_options(test_connection_pool PUBLIC "-g")

set_target_properties(
  test_connection_pool
  PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}
             LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}
             RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
