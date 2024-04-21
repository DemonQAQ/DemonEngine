#version 410 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform sampler2D texture_normal;
uniform sampler2D texture_ambientOcclusion;
uniform sampler2D texture_emissive;

uniform vec3 lightPos;  // 假设有一个光源位置
uniform vec3 viewPos;   // 相机位置

void main()
{
    // 获取各种贴图数据
    vec3 norm = texture(texture_normal, TexCoords).rgb;
    vec3 diffuse = texture(texture_diffuse, TexCoords).rgb;
    vec3 specular = texture(texture_specular, TexCoords).rgb;
    vec3 ambientOcclusion = texture(texture_ambientOcclusion, TexCoords).rgb;
    vec3 emissive = texture(texture_emissive, TexCoords).rgb;

    // 简单的光照模型计算
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 viewDir = normalize(viewPos - FragPos);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    vec3 result = (diffuse * diff + specular * spec) * ambientOcclusion + emissive;
    FragColor = vec4(result, 1.0);
}