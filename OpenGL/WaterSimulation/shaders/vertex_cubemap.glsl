#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;             // 输入的是立方体贴图的顶点坐标, 直接用立方体的顶点坐标作为纹理坐标

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;   // 使顶点的z值永远等于w值， 保证NDC中所有点的深度都是1
}