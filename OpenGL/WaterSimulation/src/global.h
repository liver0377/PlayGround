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
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
         };

// 天空图的6个面的纹理路径
inline std::vector<std::string> cubemap_faces = {
    "../assets/cubemap/right.jpg",
    "../assets/cubemap/left.jpg",
    "../assets/cubemap/up.jpg",
    "../assets/cubemap/down.jpg",
    "../assets/cubemap/front.jpg",
    "../assets/cubemap/back.jpg"};  // 右, 左, 上, 下, 后, 前

// 高度图, 地面纹理， 水纹理的纹理路径
inline std::string height_map_path = "../assets/height_map.jpg";
inline std::string ground_texture_path = "../assets/ground.jpg";
inline std::string water_texture_path = "../assets/water.png";

// 地面顶点矩阵的每个格子的大小
inline const float rec_width = 0.5f;

// 鼠标的当前位置
inline float mouse_x = SCR_WIDTH / 2;
inline float mouse_y = SCR_HEIGHT / 2;

// field of view
inline float fov = 45.0f;
inline unsigned char mouse_action = 0;

inline float water_plane_y = -1.0f;        // 水面的初始世界坐标系高度
inline float opt_speed = 0.8f;            // 海浪的速度
inline float opt_amount = 0.01f;          // 海浪的数量
inline float opt_height = 0.5f;           // 海浪的高度
inline unsigned char opt_mesh = 0;

enum class CameraMovement { FORWARD, BACKWARD, LEFT, RIGHT };

// Default camera values
inline const float YAW = -90.0f;  // yaw一开始为-90°保证视角指向z轴负向
inline const float PITCH = 0.0f;
inline const float SPEED = 50.0f;
inline const float SENSITIVITY = 0.2f;
inline const float ZOOM = 45.0f;

inline float last_time = 0.0f;

}  // namespace Global

}  // namespace Simulation

#endif