//
// Created by Demon on 2024/5/13.
//

#ifndef DEMONENGINE_TEXTURELOADER_HPP
#define DEMONENGINE_TEXTURELOADER_HPP

#include "core/base/interface/Interface.hpp"
#include "core/assets/interface/IFileLoader.hpp"
#include "core/base/render/Texture.hpp"

namespace assets
{
    class TextureLoader : implements IFileLoader
    {
    public:
        static std::shared_ptr<base::Texture> loadTexture(const std::string &path, bool isAssets = false, bool isCube = false);

        static base::TextureType extractTypeNameFromPath(const std::string &path);
    };

} // assets

#endif //DEMONENGINE_TEXTURELOADER_HPP
