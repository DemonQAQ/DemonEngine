#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 Normal;       // 输出到片段着色器的法线
out vec3 FragPos;      // 输出到片段着色器的片段位置
out vec2 TexCoords;    // 输出到片段着色器的纹理坐标

uniform mat4 model;
uniform mat4 vp;

void main()
{
    TexCoords = aTexCoords;
    vec4 worldPosition = model * vec4(aPos, 1.0); // 世界空间位置
    FragPos = vec3(worldPosition);               // 传递世界空间位置到片段着色器

    Normal = mat3(transpose(inverse(model))) * aNormal; // 正确变换法线到世界空间
    gl_Position = vp * worldPosition; // 计算裁剪空间位置
}
//#version 410 core
//layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aNormal;
//layout (location = 2) in vec2 aTexCoords;
//
//out vec2 TexCoords;
//
//uniform mat4 model;
//uniform mat4 vp;
//
//void main()
//{
//    TexCoords = aTexCoords;
//    gl_Position = vp * model * vec4(aPos, 1.0);
//}