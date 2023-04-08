#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
#include <vector>

namespace Simulation {
namespace Global {
// 窗口像素宽高
inline const int SCR_WIDTH = 800;
inline const int SCR_HEIGHT = 600;

// 天空盒的顶点(36个)数据
inline float skyboxVertices[] = {
    -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
    1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

    -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
    -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

    1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

    -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

    -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
    1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};

// 天空图的6个面的纹理
inline std::vector<std::string> cubemap_faces = {
    "../assets/cubemap/right.jpg",
    "../assets/cubemap/left.jpg",
    "../assets/cubemap/up.jpg",
    "../assets/cubemap/down.jpg",
    "../assets/cubemap/front.jpg",
    "../assets/cubemap/back.jpg"};  // 右, 左, 上, 下, 后, 前

inline float mouse_x = SCR_WIDTH / 2;
inline float mouse_y = SCR_HEIGHT / 2;
inline float fov = 45.0f;
inline unsigned char mouse_action = 0;

inline float opt_speed = 0.8f;
inline float opt_amount = 0.01f;
inline float opt_height = 0.5f;
inline unsigned char opt_mesh = 0;

enum class CameraMovement { FORWARD, BACKWARD, LEFT, RIGHT };

// Default camera values
inline const float YAW = -90.0f;  // yaw一开始为-90°保证视角指向z轴负向
inline const float PITCH = 0.0f;
inline const float SPEED = 2.5f;
inline const float SENSITIVITY = 0.1f;
inline const float ZOOM = 45.0f;
}  // namespace Global

}  // namespace Simulation

#endif