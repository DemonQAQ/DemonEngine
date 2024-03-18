//
// Created by Demon on 2023/12/14.
//

#ifndef DEMONENGINE_DRAWCALL_HPP
#define DEMONENGINE_DRAWCALL_HPP

#include <memory>
#include "core/base/render/Shader.hpp"
#include "core/base/render/Mesh.hpp"
#include "core/render/graphApi/GraphApi.hpp"

namespace render
{
    class DrawCall
    {
    public:
        std::shared_ptr<base::Shader> shader; // 使用的着色器
        std::vector<std::shared_ptr<base::Mesh>> meshes; // 要绘制的网格集合
        std::shared_ptr<base::Material> material; // 使用的材质

        // 渲染参数
        glm::mat4 modelMatrix = glm::mat4(1.0f); // 模型变换矩阵
        glm::mat4 viewMatrix = glm::mat4(1.0f); // 视图矩阵
        glm::mat4 projectionMatrix = glm::mat4(1.0f); // 投影矩阵

        // 渲染状态
        BlendMode blendMode = BlendMode::ALPHA; // 混合模式
        bool depthTestEnabled = true; // 深度测试是否启用

        DrawCall(std::shared_ptr<base::Shader> shader,
                 const std::vector<std::shared_ptr<base::Mesh>> &meshes,
                 std::shared_ptr<base::Material> material)
                : shader(shader), meshes(meshes), material(material)
        {}

        // 其他方法...
    };

}
#endif //DEMONENGINE_DRAWCALL_HPP
