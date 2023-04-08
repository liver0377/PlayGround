#include "camera/camera.h"

namespace Simulation {
/**
 * @brief
 *
 * @param position 摄像机的初始位置
 * @param up       摄像机的头顶向量
 * @param yaw      俯仰角
 * @param pitch    偏航角
 */
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
      MovementSpeed(Global::SPEED),
      MouseSensitivity(Global::SENSITIVITY),
      Zoom(Global::ZOOM),
      Position(position),
      WorldUp(up),
      Yaw(yaw),
      Pitch(pitch) {
  updateCameraVectors();
}

/**
 * @brief
 *
 * @param posX   摄像机的x坐标
 * @param posY   摄像机的y坐标
 * @param posZ   摄像机的z坐标
 * @param upX    摄像机的顶向量x分量坐标
 * @param upY    摄像机的顶向量y分量坐标
 * @param upZ    摄像机的顶向量z分量坐标
 * @param yaw    俯仰角
 * @param pitch  巡航叫
 */
Camera::Camera(float posX, float posY, float posZ, float upX, float upY,
               float upZ, float yaw, float pitch) {
  Position = glm::vec3(posX, posY, posZ);
  WorldUp = glm::vec3(upX, upY, upZ);
  Yaw = yaw;
  Pitch = pitch;
  updateCameraVectors();
}

/**
 * @brief  返回视图矩阵, 这里摄像机的视角始终指向Front
 *
 * @return glm::mat4
 */
glm::mat4 Camera::GetViewMatrix() {
  return glm::lookAt(Position, Position + Front, Up);
}

/**
 * @brief 处理键盘事件
 *
 * @param direction 前进方向
 * @param deltaTime 当前帧与上一帧的间隔时间
 */
void Camera::ProcessKeyboard(Global::CameraMovement direction,
                             float deltaTime) {
  float velocity = MovementSpeed * deltaTime;
  if (direction == Global::CameraMovement::FORWARD)
    Position += Front * velocity;
  if (direction == Global::CameraMovement::BACKWARD)
    Position -= Front * velocity;
  if (direction == Global::CameraMovement::LEFT) Position -= Right * velocity;
  if (direction == Global::CameraMovement::RIGHT) Position += Right * velocity;
}

/**
 * @brief 处理鼠标事件
 *
 * @param xoffset 鼠标x偏移量
 * @param yoffset 鼠标y偏移量
 * @param constrainPitch 是否限制俯仰角
 */
void Camera::ProcessMouseMovement(float xoffset, float yoffset,
                                  GLboolean constrainPitch) {
  xoffset *= MouseSensitivity;
  yoffset *= MouseSensitivity;

  Yaw += xoffset;
  Pitch += yoffset;

  if (constrainPitch) {
    if (Pitch > 89.0f) Pitch = 89.0f;
    if (Pitch < -89.0f) Pitch = -89.0f;
  }

  updateCameraVectors();
}

/**
 * @brief 处理鼠标滚轮事件
 *
 * @param yoffset
 */
void Camera::ProcessMouseScroll(float yoffset) {
  Zoom -= (float)yoffset;
  if (Zoom < 1.0f) Zoom = 1.0f;
  if (Zoom > 45.0f) Zoom = 45.0f;
}

/**
 * @brief 更新摄像机的前, 右, 上向量
 *
 */
void Camera::updateCameraVectors() {
  glm::vec3 front;
  front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  front.y = sin(glm::radians(Pitch));
  front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  Front = glm::normalize(front);
  Right = glm::normalize(glm::cross(Front, WorldUp));
  Up = glm::normalize(
      glm::cross(Right, Front));  // 这里其实UP还是保持WorldUP没有改变
}

}  // namespace Simulation