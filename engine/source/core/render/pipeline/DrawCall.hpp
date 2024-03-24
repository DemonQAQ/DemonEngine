//
// Created by Demon on 2023/12/14.
//

#ifndef DEMONENGINE_DRAWCALL_HPP
#define DEMONENGINE_DRAWCALL_HPP

#include <memory>
#include "core/base/render/Shader.hpp"
#include "core/base/render/Mesh.hpp"
#include "core/render/graphApi/GraphApi.hpp"
#include "core/base/render/Light.hpp"

namespace render
{
    struct RenderData
    {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        glm::mat4 globalTransform;
        std::vector<base::Vertex> vertices;
        std::vector<unsigned int> indices;
    };

    interface DrawCall
    {
    public:
        std::shared_ptr<base::Shader> shader;
        std::shared_ptr<base::Material> material;

        std::vector<RenderData> data;

        std::vector<base::Light> lights;
        glm::vec3 ambientLight = glm::vec3(0.2f, 0.2f, 0.2f);

        BlendMode blendMode = BlendMode::BLEND;
        DepthFunction depthFunction = DepthFunction::LESS;
        CullFace cullFace = CullFace::BACK;

        virtual void preExecute() = 0;
        virtual void afterExecute() = 0;
    };

}
#endif //DEMONENGINE_DRAWCALL_HPP
