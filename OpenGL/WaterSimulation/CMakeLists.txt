cmake_minimum_required(VERSION 3.23.1)

project(WaterSimulation)

set (CXX_STANDARD 17)

add_executable(water_simulation 
               ${PROJECT_SOURCE_DIR}/src/main.cc
               ${PROJECT_SOURCE_DIR}/third_party/glad/glad.cc
               ${PROJECT_SOURCE_DIR}/src/camera/camera.cc
               ${PROJECT_SOURCE_DIR}/src/shader/shader.cc
               ${PROJECT_SOURCE_DIR}/src/callbacks.cc
)

target_include_directories(water_simulation 
                           PUBLIC ${PROJECT_SOURCE_DIR}/third_party
                           PUBLIC ${PROJECT_SOURCE_DIR}/src
                         )

target_link_directories(water_simulation PUBLIC ${PROJECT_SOURCE_DIR}/libs)

target_link_libraries(water_simulation glfw3)