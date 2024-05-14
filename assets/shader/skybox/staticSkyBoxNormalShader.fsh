//#version 410 core
//out vec4 FragColor;
//
//in vec3 TexCoords;
//
//uniform samplerCube skybox;
//
//void main()
//{
//    if (TexCoords.x > 0.1) {
//        FragColor = vec4(1.0, 0.0, 0.0, 1.0); // 红色表示TexCoords.x > 0.5
//    } else {
//        FragColor = texture(skybox, TexCoords); // 否则正常显示天空盒纹理
//    }
//}
#version 410 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
    FragColor = texture(skybox, TexCoords);
    //FragColor = vec4((TexCoords + 1.0) * 0.5, 1.0);

//    vec3 skyColor = vec3(0.4, 0.7, 0.9);  // 蓝色天空
//    vec3 groundColor = vec3(0.3, 0.5, 0.55);  // 深黑色地面
//
//    // 将TexCoords.y从[-1,1]映射到[0,1]
//    float t = (TexCoords.y + 1.0) * 0.5;
//
//    // 使用smoothstep平滑过渡阈值
//    // smoothstep(edge0, edge1, x) - edge0 < edge1 为过渡区间
//    float factor = smoothstep(0.3, 0.45, t);
//
//    // 从地面颜色到天空颜色线性插值
//    vec3 color = mix(groundColor, skyColor, factor);
//
//    FragColor = vec4(color, 1.0);
}
