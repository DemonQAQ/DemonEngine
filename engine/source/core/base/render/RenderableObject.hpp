//
// Created by Demon on 2023/12/15.
//

#ifndef DEMONENGINE_RENDERABLEOBJECT_HPP
#define DEMONENGINE_RENDERABLEOBJECT_HPP


#include <memory>
#include "Model.hpp"
#include "Shader.hpp"
#include "core/base/interface/Interface.hpp"
#include "core/base/common/Object.hpp"

namespace base
{
    class RenderableObject : implements Object
    {
    public:
        std::unique_ptr <Model> model;
        std::unique_ptr <Shader> shader;

        RenderableObject(std::unique_ptr <Model> model, std::unique_ptr <Shader> shader)
                : model(std::move(model)), shader(std::move(shader))
        {}

        void render()
        {

        }

        // 其他方法，如 update, setPosition, setRotation 等
    };
}

#endif //DEMONENGINE_RENDERABLEOBJECT_HPP
