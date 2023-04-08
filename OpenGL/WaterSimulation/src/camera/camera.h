#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "global.h"

namespace Simulation {

class Camera {
 public:
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;

  // 欧拉角
  float Yaw;
  float Pitch;

  // 相机选项
  float MovementSpeed;     // 键盘控制移动速度
  float MouseSensitivity;  // 鼠标灵敏度
  float Zoom;              // fov

  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = Global::YAW,
         float pitch = Global::PITCH);

  Camera(float posX, float posY, float posZ, float upX, float upY, float upZ,
         float yaw, float pitch);

  glm::mat4 GetViewMatrix();

  void ProcessKeyboard(Global::CameraMovement direction, float deltaTime);
  void ProcessMouseMovement(float xoffset, float yoffset,
                            GLboolean constrainPitch = true);
  void ProcessMouseScroll(float yoffset);

 private:
  void updateCameraVectors();
};

}  // namespace Simulation
#endif