//
// Created by Demon on 2024/5/11.
//

#ifndef DEMONENGINE_COMMONSHAPEMANAGER_HPP
#define DEMONENGINE_COMMONSHAPEMANAGER_HPP

#include "core/base/interface/Interface.hpp"
#include "core/base/interface/Initializer.hpp"
#include "core/assets/scene/render/shape/AngularSphere.hpp"
#include "core/assets/scene/render/shape/Annulus.hpp"
#include "core/assets/scene/render/shape/Centrum.hpp"
#include "core/assets/scene/render/shape/Cube.hpp"
#include "core/assets/scene/render/shape/Cylinder.hpp"
#include "core/assets/scene/render/shape/Monkey.hpp"
#include "core/assets/scene/render/shape/Sphere.hpp"
#include "core/assets/scene/render/shape/Plane.hpp"

namespace scene
{
    class CommonShapeManager : implements base::Initializer
    {
        static std::shared_ptr<render::ICommonShape>
        getCommonShape(const std::shared_ptr<base::UUID> &existingUuid, bool init, std::shared_ptr<io::YamlConfiguration> &yml,
                       render::ShapeType shapeType)
        {
            switch (shapeType)
            {
                case render::ANGULAR_SPHERE:
                    return std::make_shared<render::AngularSphere>(existingUuid, init, yml);
                case render::ANNULUS:
                    return std::make_shared<render::Annulus>(existingUuid, init, yml);
                case render::CUBE:
                    return std::make_shared<render::Cube>(existingUuid, init, yml);
                case render::CYLINDER:
                    return std::make_shared<render::Cylinder>(existingUuid, init, yml);
                case render::CENTRUM:
                    return std::make_shared<render::Centrum>(existingUuid, init, yml);
                case render::MONKEY:
                    return std::make_shared<render::Monkey>(existingUuid, init, yml);
                case render::PLANE:
                    return std::make_shared<render::Plane>(existingUuid, init, yml);
                case render::SPHERE:
                    return std::make_shared<render::Sphere>(existingUuid, init, yml);
            }
            return nullptr;
        }

        bool init(const std::vector<std::any> &params) override
        {
            if (isInit())return false;
            render::AngularSphere::init();
            render::Annulus::init();
            render::Centrum::init();
            render::Cube::init();
            render::Cylinder::init();
            render::Monkey::init();
            render::Plane::init();
            render::Sphere::init();
            setInit(true);
            return true;
        };

        void finalize() override
        {
            if (!isInit())return;
            render::AngularSphere::finalize();
            render::Annulus::finalize();
            render::Centrum::finalize();
            render::Cube::finalize();
            render::Cylinder::finalize();
            render::Monkey::finalize();
            render::Plane::finalize();
            render::Sphere::finalize();
            setInit(false);
        };

    };
}

#endif //DEMONENGINE_COMMONSHAPEMANAGER_HPP
