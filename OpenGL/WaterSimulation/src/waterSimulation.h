#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "glad/glad.h"
#include "global.h"

#define STB_IMAGE_IMPLEMENTATION
#include <string>
#include <vector>
#include <tuple>

#include "callbacks.h"
#include "stb_image.h"
#include "shader/shader.h"
#include "camera/camera.h"



GLFWwindow* OpenGLInit();
std::tuple<unsigned int, unsigned int> renderSkyBox();
unsigned int load_cubemap_points();
unsigned int load_cubemap_texture(std::vector<std::string>& faces);
void processInput(GLFWwindow *window);