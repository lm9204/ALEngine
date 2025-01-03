#ifndef MATERIAL_H
#define MATERIAL_H

#include "Core/Base.h"
#include "Renderer/Common.h"
#include "Renderer/Object.h"

namespace ale
{

struct Albedo {
    glm::vec3 albedo = glm::vec3(1.0f);
    std::shared_ptr<Texture> albedoTexture = nullptr;
    bool flag = false;
};

struct NormalMap {
    std::shared_ptr<Texture> normalTexture = nullptr;
    bool flag = false;
};

struct Roughness {
    float roughness = 0.5f;
    std::shared_ptr<Texture> roughnessTexture = nullptr;
    bool flag = false;
};

struct Metallic {
    float metallic = 0.0f;
    std::shared_ptr<Texture> metallicTexture = nullptr;
    bool flag = false;
};

struct AOMap {
    float ao = 1.0f;
    std::shared_ptr<Texture> aoTexture = nullptr;
    bool flag = false;
};

struct HeightMap {
    float height = 0.0f;
    std::shared_ptr<Texture> heightTexture = nullptr;
    bool flag = false;
};

class AL_API Material {
public:
    static std::shared_ptr<Material> createMaterial(Albedo albedo, NormalMap normalMap, 
    Roughness roughness, Metallic metallic, AOMap aoMap, HeightMap heightMap);

    ~Material() {}

    Albedo& getAlbedo() { return albedo; }
    NormalMap& getNormalMap() { return normalMap; }
    Roughness& getRoughness() { return roughness; }
    Metallic& getMetallic() { return metallic; }
    AOMap& getAOMap() { return aoMap; }
    HeightMap& getHeightMap() { return heightMap; }

private:
    Material() = default;

    Albedo albedo;
    NormalMap normalMap;
    Roughness roughness;
    Metallic metallic;
    AOMap aoMap;
    HeightMap heightMap;

    void initMaterial(Albedo albedo, NormalMap normalMap, 
    Roughness roughness, Metallic metallic, AOMap aoMap, HeightMap heightMap);
};

}

#endif