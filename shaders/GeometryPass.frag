#version 450

// 통합 Uniform 구조체
layout(binding = 2) uniform GeometryPassFragmentUniformBufferObject {
    vec4 albedoValue;  // vec4는 16바이트 정렬이므로 먼저 배치
    float roughnessValue;
    float metallicValue;
    float aoValue;

    bool albedoFlag;
    bool normalFlag;
    bool roughnessFlag;
    bool metallicFlag;
    bool aoFlag;
} ubo;

// Textures
layout(binding = 3) uniform sampler2D normalTex;
layout(binding = 4) uniform sampler2D roughnessTex;
layout(binding = 5) uniform sampler2D metallicTex;
layout(binding = 6) uniform sampler2D aoTex;
layout(binding = 7) uniform sampler2D albedoTex;

// Inputs from Vertex Shader
layout(location = 0) in vec3 fragPosition;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragTexCoord;
layout(location = 3) in mat3 fragTBN;

// Outputs to Light Pass
layout(location = 0) out vec4 outPosition;
layout(location = 1) out vec4 outNormal;
layout(location = 2) out vec4 outAlbedo;
layout(location = 3) out vec4 outPBR;

void main() {
    // Position Pass
    outPosition = vec4(fragPosition, 1.0);

    // Normal Pass (Texture or Vertex Shader 전달)
    vec3 normal = normalize(fragNormal);
    if (ubo.normalFlag) {
        vec3 normalTexValue = texture(normalTex, fragTexCoord).rgb * 2.0 - 1.0;
        normal = normalize(fragTBN * normalTexValue);
    }
    outNormal = vec4(normal, 1.0);

    // Albedo Pass
    vec4 albedo = ubo.albedoFlag ? texture(albedoTex, fragTexCoord) : ubo.albedoValue;
    outAlbedo = albedo;

    // PBR Pass (Roughness, Metallic, AO)
    float roughness = ubo.roughnessFlag ? texture(roughnessTex, fragTexCoord).r : ubo.roughnessValue;
    float metallic = ubo.metallicFlag ? texture(metallicTex, fragTexCoord).r : ubo.metallicValue;
    float ao = ubo.aoFlag ? texture(aoTex, fragTexCoord).r : ubo.aoValue;

    outPBR = vec4(roughness, metallic, ao, 1.0);
}
