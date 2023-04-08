#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "glad/glad.h"
#include "global.h"

#define STB_IMAGE_IMPLEMENTATION
#include <string>
#include <vector>
#include <tuple>

#include "stb_image.h"
#include "shader/shader.h"
#include "camera/camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

GLFWwindow* OpenGLInit();
std::tuple<unsigned int, unsigned int> renderSkyBox();
unsigned int load_cubemap_points();
unsigned int load_cubemap_texture(std::vector<std::string>& faces);