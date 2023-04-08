#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

// 接受立方体顶点坐标作为纹理坐标
// 使用skybox作为纹理
void main()
{
    FragColor = texture(skybox, TexCoords);
}