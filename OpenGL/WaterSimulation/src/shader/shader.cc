#include "shader/shader.h"

namespace Simulation {
/**
 * @brief 创建一个shader对象
 *
 * @param vertexPath 顶点着色器路径
 * @param fragmentPath 片段着色器路径
 */
Shader::Shader(const char *vertexPath, const char *fragmentPath) {
  std::string vertexShaderSource;
  std::string fragmentShaderSource;
  std::ifstream vertexFileStream;
  std::ifstream fragmentFileStream;

  // 设置ifstream的错误掩码
  vertexFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fragmentFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    vertexFileStream.open(vertexPath);
    fragmentFileStream.open(fragmentPath);

    std::stringstream vertexStringStream;
    std::stringstream fragmentStringStream;

    vertexStringStream << vertexFileStream.rdbuf();
    fragmentStringStream << fragmentFileStream.rdbuf();

    vertexShaderSource = vertexStringStream.str();
    fragmentShaderSource = fragmentStringStream.str();

  } catch (std::ifstream::failure) {
    std::cout << "ERROR::SHDAER::FILE_NOT_SUCCESSFULY_READ" << std::endl;
  }

  // 得到shader文件的char*指针
  const char *vertexSource = vertexShaderSource.c_str();
  const char *fragmentSource = fragmentShaderSource.c_str();

  unsigned int vertex, fragment;
  vertex = glCreateShader(GL_VERTEX_SHADER);
  fragment = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(vertex, 1, &vertexSource, NULL);
  glShaderSource(fragment, 1, &fragmentSource, NULL);

  // 编译shader文件
  char info[512];
  int success;

  glCompileShader(vertex);
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex, sizeof(info), NULL, info);
    std::cout << info << std::endl;
  }

  glCompileShader(fragment);
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment, sizeof(info), NULL, info);
    std::cout << info << std::endl;
  }

  // 连接shader, 得到program对象
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);

  std::fill(info, info + sizeof(info), 0);
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(ID, sizeof(info), NULL, info);
    std::cout << info << std::endl;
  }

  // 删除shader
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

// 使用shader
void Shader::use() { glUseProgram(ID); }

void Shader::setBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const {
  glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string &name, float x, float y) const {
  glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
  glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const {
  glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const {
  glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string &name, float x, float y, float z,
                     float w) const {
  glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const {
  glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                     &mat[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const {
  glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                     &mat[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                     glm::value_ptr(mat));
}

/**
 * @brief 更新shader的视图阵以及投影阵
 * 
 * @param fov 
 * @param SRC_WIDTH 
 * @param SRC_HEIGHT 
 * @param view_matrix 
 * @param cubemap  是否正在更新cubemap
 */
void Shader::updateViewAndPesp(float fov, float SRC_WIDTH, float SRC_HEIGHT,
                        glm::mat4 view_matrix, bool cubemap) {
  glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SRC_WIDTH / (float)SRC_HEIGHT, 0.1f, 1000.0f);
  this->setMat4("projection", projection);

  if (cubemap) {
    view_matrix = glm::mat4(glm::mat3(view_matrix));
  }

  this->setMat4("view", view_matrix);
}
}  // namespace Simulation