#version 330 core
// 顶点着色器从VBO中接受顶点坐标以及纹理坐标
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

// 向片段着色器传递顶点坐标以及纹理坐标
out vec2 TexCoord;
out vec3 vertexPos;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    vertexPos = aPos;

    gl_Position = projection * view * vec4(aPos.xyz, 1.0f);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}