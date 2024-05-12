//
// Created by Demon on 2024/5/11.
//

#ifndef DEMONENGINE_COMMONSHAPEMANAGER_HPP
#define DEMONENGINE_COMMONSHAPEMANAGER_HPP

#include <memory>
#include "core/base/interface/Interface.hpp"
#include "core/base/interface/Initializer.hpp"
#include "core/base/scene/interfaace/ICommonShape.hpp"
#include "core/base/scene/shape/AngularSphere.hpp"
#include "core/base/scene/shape/Annulus.hpp"
#include "core/base/scene/shape/Cube.hpp"
#include "core/base/scene/shape/Cylinder.hpp"
#include "core/base/scene/shape/Centrum.hpp"
#include "core/base/scene/shape/Monkey.hpp"
#include "core/base/scene/shape/Plane.hpp"
#include "core/base/scene/shape/Sphere.hpp"

namespace base
{
    class CommonShapeManager : implements base::Initializer
    {
        static std::shared_ptr<base::ICommonShape>
        getCommonShape(const std::shared_ptr<base::UUID> &existingUuid, bool init, std::shared_ptr<io::YamlConfiguration> &yml,
                       base::ShapeType shapeType)
        {
            switch (shapeType)
            {
                case base::ANGULAR_SPHERE:
                    return std::make_shared<base::AngularSphere>(existingUuid, init, yml);
                case base::ANNULUS:
                    return std::make_shared<base::Annulus>(existingUuid, init, yml);
                case base::CUBE:
                    return std::make_shared<base::Cube>(existingUuid, init, yml);
                case base::CYLINDER:
                    return std::make_shared<base::Cylinder>(existingUuid, init, yml);
                case base::CENTRUM:
                    return std::make_shared<base::Centrum>(existingUuid, init, yml);
                case base::MONKEY:
                    return std::make_shared<base::Monkey>(existingUuid, init, yml);
                case base::PLANE:
                    return std::make_shared<base::Plane>(existingUuid, init, yml);
                case base::SPHERE:
                    return std::make_shared<base::Sphere>(existingUuid, init, yml);
            }
            return nullptr;
        }

        bool init(const std::vector<std::any> &params) override
        {
            if (isInit())return false;
            base::AngularSphere::init();
            base::Annulus::init();
            base::Centrum::init();
            base::Cube::init();
            base::Cylinder::init();
            base::Monkey::init();
            base::Plane::init();
            base::Sphere::init();
            setInit(true);
            return true;
        };

        void finalize() override
        {
            if (!isInit())return;
            base::AngularSphere::finalize();
            base::Annulus::finalize();
            base::Centrum::finalize();
            base::Cube::finalize();
            base::Cylinder::finalize();
            base::Monkey::finalize();
            base::Plane::finalize();
            base::Sphere::finalize();
            setInit(false);
        };

    };
}

#endif //DEMONENGINE_COMMONSHAPEMANAGER_HPP
