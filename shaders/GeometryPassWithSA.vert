#version 450

#include "../AL/include/Renderer/Animation/Bones.h"

layout(binding = 0) uniform GeometryPassVertexUniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    mat4 finalJointsMatrices[MAX_BONES];
    bool heightFlag;
    float heightScale;
} ubo;

layout(binding = 1) uniform sampler2D heightMap;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inTangent;
layout(location = 4) in ivec4 inBoneIds;
layout(location = 5) in vec4 inWeights;

layout(location = 0) out vec3 fragPosition;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec2 fragTexCoord;
layout(location = 3) out mat3 fragTBN;

void main() {
    vec4 animatedPosition = vec4(1.0f);
	mat4 boneTransform = mat4(0.0f);
	for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
	{
		if (inWeights[i] == 0)
			continue;
		if (inBoneIds[i] >= MAX_BONES) 
		{
			animatedPosition = vec4(inPosition, 1.0f);
			boneTransform = mat4(1.0f);
			break;
		}
		vec4 localPosition  = ubo.finalJointsMatrices[inBoneIds[i]] * vec4(inPosition, 1.0f);
		animatedPosition += localPosition * inWeights[i];
		boneTransform += ubo.finalJointsMatrices[inBoneIds[i]] * inWeights[i];
	}
    if (animatedPosition == vec4(1.0f) && determinant(mat3(boneTransform)) == 0.0)
    {
        animatedPosition = vec4(inPosition, 1.0f);
        boneTransform = mat4(1.0f);
    }

    if (ubo.heightFlag) {
        float height = texture(heightMap, inTexCoord).r;
        animatedPosition += vec4(inNormal * (height * ubo.heightScale), 0.0f);
    }

    vec4 positionWorld = ubo.model * animatedPosition;
    gl_Position = ubo.proj * ubo.view * positionWorld;
    fragPosition = positionWorld.xyz;



    mat3 normalMatrix = transpose(inverse(mat3(ubo.model) * mat3(boneTransform)));
    fragNormal = normalMatrix * inNormal;

    fragTexCoord = inTexCoord;

    vec3 T = normalize(normalMatrix * inTangent);
    vec3 N = normalize(fragNormal);
    vec3 B = normalize(cross(N, T));

    fragTBN = mat3(T, B, N);
}
