#version 410 core
out vec4 FragColor;

//in vec3 Normal;
//in vec3 FragPos;
in vec2 TexCoords;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform sampler2D texture_normal;
uniform sampler2D texture_ambientOcclusion;
uniform sampler2D texture_emissive;

uniform bool has_texture_diffuse;
uniform bool has_texture_specular;
uniform bool has_texture_normal;
uniform bool has_texture_ambientOcclusion;
uniform bool has_texture_emissive;

void main()
{
    FragColor = texture(texture_diffuse, TexCoords);
}

//uniform vec3 lightPos;  // 假设有一个光源位置
//uniform vec3 viewPos;   // 相机位置

//void main()
//{
//    vec3 norm = has_texture_normal ? texture(texture_normal, TexCoords).rgb : vec3(1.0, 1.0, 1.0);
//    vec3 diffuse = has_texture_diffuse ? texture(texture_diffuse, TexCoords).rgb : vec3(1.0, 1.0, 1.0);
//    vec3 specular = has_texture_specular ? texture(texture_specular, TexCoords).rgb : vec3(0.0, 0.0, 0.0);
//    vec3 ambientOcclusion = has_texture_ambientOcclusion ? texture(texture_ambientOcclusion, TexCoords).rgb : vec3(1.0, 1.0, 1.0);
//    vec3 emissive = has_texture_emissive ? texture(texture_emissive, TexCoords).rgb : vec3(0.0, 0.0, 0.0);
//
//    vec3 lightDir = normalize(lightPos - FragPos);
//    float diff = max(dot(norm, lightDir), 0.0);
//    vec3 reflectDir = reflect(-lightDir, norm);
//    vec3 viewDir = normalize(viewPos - FragPos);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
//
//    vec3 result = (diffuse * diff + specular * spec) * ambientOcclusion + emissive;
//    FragColor = vec4(result, 1.0);
//    FragColor = texture(texture_diffuse, TexCoords);
//}
