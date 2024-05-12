//
// Created by Demon on 2024/3/7.
//

#ifndef DEMONENGINE_ASSETTYPE_HPP
#define DEMONENGINE_ASSETTYPE_HPP

#include <string>
#include <map>

namespace assets
{
    enum class AssetType
    {
        MATERIALS,
        MEDIA,
        MODEL,
        RENDERABLE_OBJECT,
        TEXTURE,
        SHADER,
        CONFIG,
        SCRIPT,
        SKY_BOX,
        NONE
    };

    class AssetTypeUtil
    {
    public:
        static AssetType GetAssetTypeByExtension(const std::string &extension)
        {
            static const std::map<std::string, AssetType> extensionToTypeMap = {
                    {".mat",   AssetType::MATERIALS},
                    {".mp3",   AssetType::MEDIA},
                    {".wav",   AssetType::MEDIA},
                    {".obj",   AssetType::MODEL},
                    {".fbx",   AssetType::MODEL},
                    {".3ds",   AssetType::MODEL},
                    {".blend", AssetType::MODEL},
                    {".dae",   AssetType::MODEL},
                    {".gltf",  AssetType::MODEL},
                    {".glb",   AssetType::MODEL},
                    {".png",   AssetType::TEXTURE},
                    {".jpg",   AssetType::TEXTURE},
                    {".jpeg",  AssetType::TEXTURE},
                    {".tga",   AssetType::TEXTURE},
                    {".bmp",   AssetType::TEXTURE},
                    {".glsl",  AssetType::SHADER},
                    {".hlsl",  AssetType::SHADER},
                    {".json",  AssetType::CONFIG},
                    {".yml",  AssetType::CONFIG},
                    {".xml",   AssetType::CONFIG},
                    {".cs",   AssetType::SCRIPT}
            };

            auto it = extensionToTypeMap.find(extension);
            if (it != extensionToTypeMap.end())
            {
                return it->second;
            }

            return AssetType::NONE;
        }
    };
}

#endif //DEMONENGINE_ASSETTYPE_HPP
