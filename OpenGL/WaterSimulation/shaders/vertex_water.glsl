#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 vertexPos;

uniform mat4 view;
uniform mat4 projection;
uniform float time;

// 海浪速度, 数量, 高度
// 海浪的起伏表现在y轴上是正弦波
// speed对应着正弦波的周期, speed越大, 周期越小, 海浪就表现地越汹涌
// amount对应
uniform float speed;
uniform float amount;
uniform float height;

void main()
{
	vertexPos = aPos;

	// float xx = (aPos.x-3) * (aPos.x-3);
	float yy = (aPos.y+1) * (aPos.y+1);

	float y = sin(time * speed + (aPos.x * aPos.z * amount) + 0.5 * cos(aPos.x * aPos.z * amount)) * height;
	gl_Position = projection * view * vec4(aPos.x, y, aPos.z, 1.0f);

	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}