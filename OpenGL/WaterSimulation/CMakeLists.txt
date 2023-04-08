cmake_minimum_required(VERSION 3.23.1)

project(WaterSimulation)

set (CXX_STANDARD 17)

find_package(glfw3 REQUIRED)

add_executable(water_simulation 
               ${PROJECT_SOURCE_DIR}/src/main.cc
               ${PROJECT_SOURCE_DIR}/glad/glad.cc
               ${PROJECT_SOURCE_DIR}/src/camera/camera.cc
               ${PROJECT_SOURCE_DIR}/src/shader/shader.cc
)

target_include_directories(water_simulation 
                           PUBLIC E:\\dev\\include 
                           PUBLIC ${PROJECT_SOURCE_DIR}/src
                         )

target_link_directories(water_simulation PUBLIC E:\\dev\\lib)

target_link_libraries(water_simulation glfw3)