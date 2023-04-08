#version 330 core
out vec4 FragColor;

in vec3 vertexPos;
in vec2 TexCoord;

uniform sampler2D TexGround;

void main() {
    FragColor = texture(TexGround, TexCoord);

    // [-10, 1.5]高度为第一层
    if (vertexPos.y <= 1.5)
        FragColor *= vec4(0.761, 0.698, 0.502, 1.0);
    // (1.5, 6]为第二层
    else if (vertexPos.y > 1.5 && vertexPos.y <= 6)
        FragColor *= vec4(0.172, 0.690, 0.215, 1.0);
    // (6, 10)为第三层
    else if (vertexPos.y > 6 && vertexPos.y < 10)
        FragColor *= vec4(0.501, 0.517, 0.529, 1.0);

    // 漫反射光
    // 环境光
    vec3 diffuse = vec3(-1.0, -1.0, -1.0);
    float attenuation =  dot(-normalize(cross(dFdx(vertexPos), dFdy(vertexPos))), diffuse);
    attenuation = max(attenuation, 0.0);

    vec3 hazy_ambiant = 0.4 * vec3(0.741, 0.745, 0.752);

    FragColor.xyz *= (hazy_ambiant + attenuation);
    FragColor.a = 1.0;
}