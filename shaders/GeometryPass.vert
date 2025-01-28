#version 450

layout(binding = 0) uniform GeometryPassVertexUniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    bool heightFlag;
    float heightScale;
} ubo;

layout(binding = 1) uniform sampler2D heightMap;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inTangent;

layout(location = 0) out vec3 fragPosition;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec2 fragTexCoord;
layout(location = 3) out mat3 fragTBN;

void main() {
    vec3 position = inPosition;

    if (ubo.heightFlag) {
        float height = texture(heightMap, inTexCoord).r;
        position += inNormal * (height * ubo.heightScale);
    }

    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(position, 1.0);
    fragPosition = vec3(ubo.model * vec4(position, 1.0));

    mat3 normalMatrix = mat3(transpose(inverse(ubo.model)));
    fragNormal = normalMatrix * inNormal;

    fragTexCoord = inTexCoord;

    vec3 T = normalize(normalMatrix * inTangent);
    vec3 N = normalize(fragNormal);
    vec3 B = normalize(cross(N, T));

    fragTBN = mat3(T, B, N);
}
