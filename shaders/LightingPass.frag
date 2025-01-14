#version 450

layout(input_attachment_index = 0, binding = 0) uniform subpassInput positionAttachment;
layout(input_attachment_index = 1, binding = 1) uniform subpassInput normalAttachment;
layout(input_attachment_index = 2, binding = 2) uniform subpassInput albedoAttachment;
layout(input_attachment_index = 3, binding = 3) uniform subpassInput pbrAttachment;

layout(binding = 4) uniform LightingInfo {
    vec3 lightPos;
    vec3 lightDirection;
    vec3 lightColor;
    vec3 cameraPos;
    float intensity;
    float ambientStrength;
};

layout(location = 0) in vec2 fragTexCoord;
layout(location = 0) out vec4 outColor;

// Fresnel-Schlick Approximation
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

// Normal Distribution Function (NDF)
float distributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    float denominator = (NdotH2 * (a2 - 1.0) + 1.0);
    return a2 / (3.14159265359 * denominator * denominator);
}

// Geometry Function
float geometrySchlickGGX(float NdotV, float roughness) {
    float k = (roughness + 1.0) * (roughness + 1.0) / 8.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    return geometrySchlickGGX(NdotV, roughness) * geometrySchlickGGX(NdotL, roughness);
}

void main() {
    vec3 fragPosition = subpassLoad(positionAttachment).rgb;
    vec3 fragNormal = normalize(subpassLoad(normalAttachment).rgb);
    // -1 ~ 1 범위로 변환 잘라버리기
    fragNormal = clamp(fragNormal, -1.0, 1.0);
    vec3 albedo = subpassLoad(albedoAttachment).rgb;

    vec4 pbr = subpassLoad(pbrAttachment);
    float roughness = pbr.r;
    float metallic = pbr.g;
    float ao = pbr.b;

    // 법선, 뷰, 라이트 벡터
    vec3 N = fragNormal;
    vec3 V = normalize(cameraPos - fragPosition);
    vec3 L = normalize(lightPos - fragPosition); // 포인트 라이트 방향
    vec3 H = normalize(V + L);

    // 거리 계산 및 감쇠 적용
    float distance = length(lightPos - fragPosition);
    float constant = 1.0;
    float linear = 0.09;
    float quadratic = 0.032;
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

    // Fresnel
    vec3 F0 = mix(vec3(0.04), albedo, metallic); // 금속성과 알베도를 고려한 반사율
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

    // NDF와 Geometry
    float NDF = distributionGGX(N, H, roughness);
    float G = geometrySmith(N, V, L, roughness);

    // 스페큘러
    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
    vec3 specular = numerator / denominator;

    // Diffuse와 Specular 비율
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic; // 금속성은 diffuse를 줄임

    // // 라이트 기여도
    // float NdotL = max(dot(N, L), 0.0);
    // vec3 radiance = lightColor * intensity * NdotL;

    // 라이트 기여도
    float NdotL = max(dot(N, L), 0.0);
    vec3 radiance = (lightColor * intensity * NdotL) * attenuation; // 감쇠 적용

    // 최종 색상
    vec3 diffuse = kD * albedo / 3.14159265359;
    vec3 ambient = ambientStrength * albedo * ao;
    vec3 finalColor = ambient + (diffuse + specular) * radiance;

    outColor = vec4(finalColor, 1.0);
    // outColor = vec4(fragNormal * 0.5 + 0.5, 1.0); // Normal
}
