#ifndef CALL_BACK_H
#define CALL_BACK_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "glad/glad.h"
#include "global.h"
#include "camera/camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

extern Simulation::Camera* camera;

#endif