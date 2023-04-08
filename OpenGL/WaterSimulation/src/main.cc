#include "waterSimulation.h"

Simulation::Camera* camera = new Simulation::Camera();

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
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "gald initialize error" << std::endl;
    exit(-1);
  }

  // ???
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // 设置回调函数
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetKeyCallback(window, key_callback);

  return window;
}

/**
 * @brief 加载天空盒的顶点, 纹理
 * 
 * @return 顶点VAO, 纹理对象ID 
 */
std::tuple<unsigned int, unsigned int> renderSkyBox() {
  // 1. 加载天空图的顶点数据
  unsigned int VAO_cubemap = LoadCubeMapPoints();

  // 2. 加载天空图的纹理
  unsigned int texture_cubemap =
      LoadCubeMapTexture(Simulation::Global::cubemap_faces);

  return {VAO_cubemap, texture_cubemap};
}

/**
 * @brief 加载地面的顶点, 纹理
 * 
 * @return VAO, 纹理ID
 */
std::tuple<unsigned int, unsigned int, unsigned int> renderGround() {
  // 1. 根据高度图得到地面的顶点数组, 索引数组
  int width, height, nChannels;
  unsigned char* data = stbi_load(Simulation::Global::height_map_path.c_str(), &width, &height, &nChannels, 0);
  std::vector<float> *vertices = InitHeightMap(data, width, height, nChannels, Simulation::Global::rec_width);
  stbi_image_free(data);
  std::vector<unsigned int>* indices = InitIndices(width, height);

  // 2. 加载顶点数据
  unsigned int VAO_ground = LoadObject(vertices, indices);

  // 3. 加载纹理
  unsigned int texture_ground = LoadTexture(Simulation::Global::ground_texture_path.c_str());

  return {VAO_ground, texture_ground, indices->size()};
}

/**
 * @brief 加载立方体贴图的顶点数据
 *
 * @return VAO
 */
unsigned int LoadCubeMapPoints() {
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
unsigned int LoadCubeMapTexture(std::vector<std::string>& faces) {
  unsigned int textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

  // 将6个面的纹理图像全部加载到texture对象中
  int width, height, nChannels;
  for (int i = 0; i < faces.size(); i++) {
    unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nChannels, 0);
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
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  // 线性过滤
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,
                  GL_CLAMP_TO_EDGE);  // 两个面交线上的点可能不会集中任何一个面的纹理，
                                      // 这里强制令其重复某一个面的纹理

  return textureID;
}



/**
 * @brief  目前只有一个事件, 按ESC窗口关闭
 * 
 * @param window 
 */
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}



/**
 * @brief  加载2D纹理
 * 
 * @param path  纹理文件路径
 *
 * @return 纹理ID
 */
unsigned int LoadTexture(const char* path) {
  unsigned int textureID;

  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);

  unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
  if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
  }
  else {
      std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  return textureID;
}

/**
 * @brief 将高度图的高度数据转换为图形的顶点数组
 * 
 * @param data   高度图的字节数组
 * @param width  高度图的高度
 * @param height  高度图的宽度
 * @param nChannels  颜色通道的数量
 * @param rec_width  单个格子的宽度
 * @return 
 */
std::vector<float>* InitHeightMap(const unsigned char*data, int width, int height, int nChannels, float rec_width) {
    float scale_y = 0.1f, shift_y = -10.0f;
    auto vertices = new std::vector<float>();

    float start_x = -(width / 2) * rec_width;
    float start_z = -(height / 2) * rec_width;

    for (int x = 0; x < width; x++)
    {
        float pos_x = start_x + x * rec_width;    // 该像素点的实际x坐标
        for (int z = 0; z < height; z++)
        {
            float pos_z = start_z + z * rec_width;   // 该像素的实际z坐标

            int pos_img = (x + z * width) * nChannels;
            float pixel = data[pos_img]; // Height map is already grayscaled

            pixel = pixel * scale_y + shift_y;
            vertices->push_back(pos_x);
            vertices->push_back(pixel);
            vertices->push_back(pos_z);

            // Texture coordinates
            // 可以将整个纹理看成是平铺在地面上
            vertices->push_back(pos_x);
            vertices->push_back(pos_z);
        }
    }

    return vertices;
}

/**
 * @brief 得到地面纹理的索引数组
 * 
 * @param width 地面顶点矩阵宽
 * @param height 地面顶点矩阵高
 * @return 索引 数组
 */
std::vector<unsigned int>* InitIndices(int width, int height) {
  std::vector<unsigned int>* indices = new std::vector<unsigned int>();
  for (int i = 0; i < height - 1; i ++) {
    for (int j = 0; j < width - 1; j ++) {
      auto top_left = j * height + i;
      auto bottom_left = top_left + 1;
      auto top_right = top_left + height;
      auto bottom_right = top_right + 1; 

      // 绘制6个顶点, 2个三角形
      indices->push_back(top_left);
      indices->push_back(top_right);
      indices->push_back(bottom_left);
      indices->push_back(bottom_left);
      indices->push_back(bottom_right);
      indices->push_back(top_right);
    }
  }

  return indices;
}

/**
 * @brief 对于给定的模型的顶点数组以及索引数组创建对应的VAO, VBO, EBO
 * 
 * @param vertices  顶点数组
 * @param indices   索引数组
 * @return VAO的ID 
 */
unsigned int LoadObject(std::vector<float>* vertices, std::vector<unsigned int>* indices) {
   unsigned int VAO, VBO, EBO;
   glGenVertexArrays(1, &VAO);
   glGenBuffers(1, &VBO);
   glGenBuffers(1, &EBO);

   glBindVertexArray(VAO);

   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof (float), &vertices->at(0), GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof (float), &indices->at(0), GL_STATIC_DRAW);

   // 解释VBO
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof (float), (void*) 0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof (float), (void*)(3 * sizeof (float)));
   glEnableVertexAttribArray(1);

   return VAO;
}

/**
 * @brief 绘制地面
 * 
 * @param shader_ground  地面shader
 * @param VAO_ground     地面VAO
 * @param ground_indices_size 地面索引数组大小, 用于地面三角形的绘制
 */
void drawGround(Simulation::Shader& shader_ground, unsigned int VAO_ground, unsigned int ground_indices_size) {
        shader_ground.use();
        shader_ground.updateViewAndPesp(Simulation::Global::fov, Simulation::Global::SCR_WIDTH,
                                        Simulation::Global::SCR_HEIGHT, camera->GetViewMatrix(), false);

        glBindVertexArray(VAO_ground);
        glDrawElements(GL_TRIANGLES, ground_indices_size, GL_UNSIGNED_INT, 0);
}

/**
 * @brief  绘制立方体贴图, 即天空图
 * 
 * @param shader_cubemap 
 * @param VAO_cubemap 
 */
void drawCubeMap(Simulation::Shader& shader_cubemap, unsigned int VAO_cubemap) {
    glDepthFunc(GL_LEQUAL);  // 由于天空盒的图片深度均为1, z-buffer内的初始值就是1，
                             // 所以必须设置为等于也可见 否则就会导致天空盒被 "遮挡"
    shader_cubemap.use();
    shader_cubemap.updateViewAndPesp(Simulation::Global::fov, Simulation::Global::SCR_WIDTH,
                                     Simulation::Global::SCR_HEIGHT, camera->GetViewMatrix(), true);

    glBindVertexArray(VAO_cubemap);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

int main() {
  GLFWwindow* window = OpenGLInit();
  

  // 1. shader对象创建
  Simulation::Shader shader_cubemap("../shaders/vertex_cubemap.glsl",
                                    "../shaders/frag_cubemap.glsl");
  Simulation::Shader shader_ground("../shaders/vertex_ground.glsl",
                                   "../shaders/frag_ground.glsl");
                          

  // 2, VAO, VBO, EBO, 纹理的设置
  // skybox
  auto [VAO_cubemap, texture_cubemap] = renderSkyBox();
  // ground
  auto [VAO_ground, texture_ground, ground_indices_size] = renderGround();

  
  // 解绑
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBindVertexArray(0);

  // 2. shader uniform变量设置
  // water, ground, cubmap的纹理单元分别为1
  shader_ground.use();
  shader_ground.setInt("TexGround", 1);

  shader_cubemap.use();
  shader_cubemap.setInt("skybox", 2);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 激活并将纹理绑定到对应的shader
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_ground);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_cubemap);

    // 绘制图形
    drawGround(shader_ground, VAO_ground, ground_indices_size);
    drawCubeMap(shader_cubemap, VAO_cubemap);

    glBindVertexArray(0);  // 解绑
    glDepthFunc(GL_LESS);  // 恢复深度函数

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}