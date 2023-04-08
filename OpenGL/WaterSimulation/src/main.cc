#include "waterSimulation.h"

/**
 * @brief 初始化glfw, glad
 *
 * @return window
 */
GLFWwindow* OpenGLInit() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(Simulation::Global::SCR_WIDTH,
                                        Simulation::Global::SCR_HEIGHT,
                                        "Water Simulation", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    exit(-1);
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "gald initialize error" << std::endl;
    exit(-1);
  }

  // ???
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // 设置回调函数
  // glfwSetMouseButtonCallback(window, mouse_button_callback);
  // glfwSetCursorPosCallback(window, mouse_callback);
  // glfwSetKeyCallback(window, key_callback);

  return window;
}

std::tuple<unsigned int, unsigned int> renderSkyBox() {
  // 1. 加载天空图的顶点数据
  unsigned int VAO_cubemap = load_cubemap_points();

  // 2. 加载天空图的纹理
  unsigned int texture_cubemap =
      load_cubemap_texture(Simulation::Global::cubemap_faces);

  return {VAO_cubemap, texture_cubemap};
}

/**
 * @brief 加载立方体贴图的顶点数据
 *
 * @return VAO
 */
unsigned int load_cubemap_points() {
  unsigned int VAO, VBO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Simulation::Global::skyboxVertices),
               &Simulation::Global::skyboxVertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  return VAO;
}

/**
 * @brief 加载立方体贴图的纹理
 *
 * @param faces 6个面的纹理路径
 * @return textureID
 */
unsigned int load_cubemap_texture(std::vector<std::string>& faces) {
  unsigned int textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

  // 将6个面的纹理图像全部加载到texture对象中
  int width, height, nrChannels;
  for (int i = 0; i < faces.size(); i++) {
    unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
    if (data) {
      // POSITIVE_X表示正右面
      // 不断累加， 顺序为右， 左， 上， 下， 后， 前
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height,
                   0, GL_RGB, GL_UNSIGNED_BYTE, data);
      stbi_image_free(data);
    } else {
      std::cout << "cubemap image load error" << std::endl;
      stbi_image_free(data);
      exit(-1);
    }
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER,
                  GL_LINEAR);  // 线性过滤
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,
                  GL_CLAMP_TO_EDGE);  // 两个面交线上的点可能不会集中任何一个面的纹理，
                                      // 这里强制令其重复某一个面的纹理

  return textureID;
}

int main() {
  GLFWwindow* window = OpenGLInit();
  Simulation::Camera* camera = new Simulation::Camera();

  // 1. shader对象创建
  Simulation::Shader shader_cubemap("../shaders/vertex_cubemap.glsl",
                                    "../shaders/frag_cubemap.glsl");

  auto [VAO_cubemap, texture_cubemap] = renderSkyBox();

  // 解绑
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBindVertexArray(0);

  // 2. shader uniform变量设置
  shader_cubemap.use();
  shader_cubemap.setInt("skybox", 2);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.2f, 0.3f, 0.3f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 激活并将纹理绑定到对应的shader
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_cubemap);

    glDepthFunc(GL_LEQUAL);  // 由于天空盒的图片深度均为1, z-buffer内的初始值就是1，
                             // 所以必须设置为等于也可见 否则就会导致天空盒被 "遮挡"
    shader_cubemap.use();
    shader_cubemap.updateViewAndPesp(Simulation::Global::fov, Simulation::Global::SCR_WIDTH,
                                     Simulation::Global::SCR_HEIGHT, camera->GetViewMatrix(), true);

    glBindVertexArray(VAO_cubemap);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);  // 解绑
    glDepthFunc(GL_LESS);  // 恢复深度函数

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}