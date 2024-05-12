//
// Created by Demon on 2024/5/11.
//

#ifndef DEMONENGINE_STATICSKYBOX_HPP
#define DEMONENGINE_STATICSKYBOX_HPP

#include "core/base/interface/ISkyBox.hpp"
#include "core/base/scene/shape/Cube.hpp"

namespace base
{
    class StaticSkybox : implements ISkyBox
    {
    private:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::shared_ptr<Texture> texture;
    public:
        StaticSkybox(const std::shared_ptr<base::UUID> &existingUuid, bool init,
                     const std::string &name, std::shared_ptr<io::YamlConfiguration> &yml,
                     SkyBoxType skyBoxType, const std::shared_ptr<Texture> &texture,
                     glm::vec3 emissiveColor = glm::vec3(0.1f, 0.1f, 0.5f),
                     const std::shared_ptr<base::UUID> &shaderUUID = nullptr) :
                ISkyBox(existingUuid, init, name, yml, skyBoxType, emissiveColor, shaderUUID, nullptr), texture(texture)
        {
            initializeVertices();

            if (shaderUUID)bindShader(shaderUUID);
        }

        void beforeRendering(const std::vector<std::any> &params) override
        {

        }

        void afterRendering(const std::vector<std::any> &params) override
        {

        }

        void getRenderData(std::vector<RenderData> &renderDataList) override
        {
            RenderData renderData;
            renderData.vertices = this->vertices;
            renderData.indices = this->indices;
            renderData.globalTransform = getGlobalTransform();
            renderData.useShader = this->getShader();
            renderData.material = nullptr;
            renderDataList.push_back(renderData);
        }

        std::shared_ptr<Texture> getTexture() const override
        {
            return texture;
        }

        void updateGlobalTransform(std::vector<Transform> &additionalTransforms) override
        {

        }

        Transform getLocalTransform() const override
        {
            return {};
        }
    protected:
        void updateObservedGlobalTransform(std::vector<Transform> &additionalTransforms) override
        {

        }
    private:
        void initializeVertices()
        {
            const float halfSize = 1.0f;
            std::vector<glm::vec3> positions = {
                    // Front face
                    {-halfSize, -halfSize, halfSize},
                    {halfSize,  -halfSize, halfSize},
                    {halfSize,  halfSize,  halfSize},
                    {-halfSize, halfSize,  halfSize},
                    // Back face
                    {halfSize,  -halfSize, -halfSize},
                    {-halfSize, -halfSize, -halfSize},
                    {-halfSize, halfSize,  -halfSize},
                    {halfSize,  halfSize,  -halfSize},
                    // Left face
                    {-halfSize, -halfSize, -halfSize},
                    {-halfSize, -halfSize, halfSize},
                    {-halfSize, halfSize,  halfSize},
                    {-halfSize, halfSize,  -halfSize},
                    // Right face
                    {halfSize,  -halfSize, halfSize},
                    {halfSize,  halfSize,  halfSize},
                    {halfSize,  halfSize,  -halfSize},
                    {halfSize,  -halfSize, -halfSize},
                    // Top face
                    {-halfSize, halfSize,  halfSize},
                    {halfSize,  halfSize,  halfSize},
                    {halfSize,  halfSize,  -halfSize},
                    {-halfSize, halfSize,  -halfSize},
                    // Bottom face
                    {-halfSize, -halfSize, -halfSize},
                    {halfSize,  -halfSize, -halfSize},
                    {halfSize,  -halfSize, halfSize},
                    {-halfSize, -halfSize, halfSize}
            };

            std::vector<glm::vec3> normals = {
                    {0.0f,  0.0f,  1.0f},
                    {0.0f,  0.0f,  1.0f},
                    {0.0f,  0.0f,  1.0f},
                    {0.0f,  0.0f,  1.0f}, // Front
                    {0.0f,  0.0f,  -1.0f},
                    {0.0f,  0.0f,  -1.0f},
                    {0.0f,  0.0f,  -1.0f},
                    {0.0f,  0.0f,  -1.0f}, // Back
                    {-1.0f, 0.0f,  0.0f},
                    {-1.0f, 0.0f,  0.0f},
                    {-1.0f, 0.0f,  0.0f},
                    {-1.0f, 0.0f,  0.0f}, // Left
                    {1.0f,  0.0f,  0.0f},
                    {1.0f,  0.0f,  0.0f},
                    {1.0f,  0.0f,  0.0f},
                    {1.0f,  0.0f,  0.0f}, // Right
                    {0.0f,  1.0f,  0.0f},
                    {0.0f,  1.0f,  0.0f},
                    {0.0f,  1.0f,  0.0f},
                    {0.0f,  1.0f,  0.0f}, // Top
                    {0.0f,  -1.0f, 0.0f},
                    {0.0f,  -1.0f, 0.0f},
                    {0.0f,  -1.0f, 0.0f},
                    {0.0f,  -1.0f, 0.0f}  // Bottom
            };

            float aspectRatio = static_cast<float>(texture->width) / (float) (texture->height / 2.0f);
            float segmentWidth = 1.0f / 6.0f;  // 1 divided by number of segments in one row
            float adjustedSegmentWidth = segmentWidth * aspectRatio;

            // Adjust UV mapping to apply aspect ratio
            std::vector<glm::vec2> uvs = {
                    // Front face
                    {0.0f * adjustedSegmentWidth, 0.0f},
                    {1.0f * adjustedSegmentWidth, 0.0f},
                    {1.0f * adjustedSegmentWidth, 1.0f},
                    {0.0f * adjustedSegmentWidth, 1.0f},
                    // Back face
                    {1.0f * adjustedSegmentWidth, 0.0f},
                    {2.0f * adjustedSegmentWidth, 0.0f},
                    {2.0f * adjustedSegmentWidth, 1.0f},
                    {1.0f * adjustedSegmentWidth, 1.0f},
                    // Left face
                    {2.0f * adjustedSegmentWidth, 0.0f},
                    {3.0f * adjustedSegmentWidth, 0.0f},
                    {3.0f * adjustedSegmentWidth, 1.0f},
                    {2.0f * adjustedSegmentWidth, 1.0f},
                    // Right face
                    {3.0f * adjustedSegmentWidth, 0.0f},
                    {4.0f * adjustedSegmentWidth, 0.0f},
                    {4.0f * adjustedSegmentWidth, 1.0f},
                    {3.0f * adjustedSegmentWidth, 1.0f},
                    // Top face
                    {4.0f * adjustedSegmentWidth, 0.0f},
                    {5.0f * adjustedSegmentWidth, 0.0f},
                    {5.0f * adjustedSegmentWidth, 1.0f},
                    {4.0f * adjustedSegmentWidth, 1.0f},
                    // Bottom face
                    {5.0f * adjustedSegmentWidth, 0.0f},
                    {6.0f * adjustedSegmentWidth, 0.0f},
                    {6.0f * adjustedSegmentWidth, 1.0f},
                    {5.0f * adjustedSegmentWidth, 1.0f}
            };

            // Construct vertex data
            for (size_t i = 0; i < positions.size(); ++i)
            {
                vertices.emplace_back(positions[i], normals[i], uvs[i]);
            }

            // Index data (two triangles per face)
            indices = {
                    0, 1, 2, 2, 3, 0,   // Front face
                    4, 5, 6, 6, 7, 4,   // Back face
                    8, 9, 10, 10, 11, 8, // Left face
                    12, 13, 14, 14, 15, 12, // Right face
                    16, 17, 18, 18, 19, 16, // Top face
                    20, 21, 22, 22, 23, 20  // Bottom face
            };
        }
    };
}

#endif //DEMONENGINE_STATICSKYBOX_HPP
