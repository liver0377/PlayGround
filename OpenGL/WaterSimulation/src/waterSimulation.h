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
std::vector<float>* InitWaterPlane(int width, int height, int water_plane_y);
std::vector<float>* InitHeightMap(const unsigned char*data, int width, int height, int nChannels, float rec_width);
std::vector<unsigned int>* InitIndices(int width, int height);
std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int> renderGround();
std::tuple<unsigned int, unsigned int> renderSkyBox();

unsigned int LoadObject(std::vector<float>* vertices, std::vector<unsigned int>* indices);
unsigned int LoadTexture(const char* path);
unsigned int LoadCubeMapPoints();
unsigned int LoadCubeMapTexture(std::vector<std::string>& faces);

void processInput(GLFWwindow *window);