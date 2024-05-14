#version 410 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 view;  // 视图矩阵
uniform mat4 proj;  // 投影矩阵

void main()
{
    TexCoords = aPos;
    mat4 rotationOnlyView = view;  // 复制视图矩阵
    rotationOnlyView[3] = vec4(0.0, 0.0, 0.0, 1.0);  // 移除视图矩阵中的平移部分

    vec4 pos = proj * rotationOnlyView * vec4(aPos, 1.0);  // 使用无平移的视图矩阵和正常的投影矩阵
    gl_Position = pos.xyww;
}
// 顶点着色器
//#version 410 core
//layout (location = 0) in vec3 aPos;
//
//void main()
//{
//    gl_Position = vec4(aPos, 1.0);  // 直接输出顶点位置作为裁剪空间位置
//}