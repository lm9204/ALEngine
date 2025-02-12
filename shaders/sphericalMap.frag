#version 450
#extension GL_ARB_shader_viewport_layer_array : enable

layout(location = 0) in vec3 outPosition;
layout(location = 0) out vec4 fragColor;

layout(binding = 1) uniform sampler2D tex;

const vec2 invPi = vec2(0.1591549, 0.3183098862);

vec2 SampleSphericalMap(vec3 v) {
    return vec2(atan(v.z, v.x), asin(v.y)) * invPi + 0.5;
}

void main() {
    vec2 uv = SampleSphericalMap(normalize(outPosition)); // normalize
    vec3 color = texture(tex, uv).rgb;
    fragColor = vec4(color, 1.0);
}
