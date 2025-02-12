#version 450
#extension GL_ARB_shader_viewport_layer_array : enable

vec3 positions[36] = vec3[](
    // **Right (+X) - layerIndex 0**
    vec3( 0.5, -0.5, -0.5), vec3( 0.5, -0.5,  0.5), vec3( 0.5,  0.5,  0.5),
    vec3( 0.5,  0.5,  0.5), vec3( 0.5,  0.5, -0.5), vec3( 0.5, -0.5, -0.5),

    // **Left (-X) - layerIndex 1**
    vec3(-0.5, -0.5, -0.5), vec3(-0.5,  0.5, -0.5), vec3(-0.5,  0.5,  0.5),
    vec3(-0.5,  0.5,  0.5), vec3(-0.5, -0.5,  0.5), vec3(-0.5, -0.5, -0.5),

    // **Top (+Y) - layerIndex 2**
    vec3(-0.5,  0.5, -0.5), vec3( 0.5,  0.5, -0.5), vec3( 0.5,  0.5,  0.5),
    vec3( 0.5,  0.5,  0.5), vec3(-0.5,  0.5,  0.5), vec3(-0.5,  0.5, -0.5),

    // **Bottom (-Y) - layerIndex 3**
    vec3(-0.5, -0.5, -0.5), vec3(-0.5, -0.5,  0.5), vec3( 0.5, -0.5,  0.5),
    vec3( 0.5, -0.5,  0.5), vec3( 0.5, -0.5, -0.5), vec3(-0.5, -0.5, -0.5),

    // **Front (+Z) - layerIndex 4**
    vec3(-0.5, -0.5,  0.5), vec3(-0.5,  0.5,  0.5), vec3( 0.5,  0.5,  0.5),
    vec3( 0.5,  0.5,  0.5), vec3( 0.5, -0.5,  0.5), vec3(-0.5, -0.5,  0.5),

    // **Back (-Z) - layerIndex 5**
    vec3(-0.5, -0.5, -0.5), vec3( 0.5, -0.5, -0.5), vec3( 0.5,  0.5, -0.5),
    vec3( 0.5,  0.5, -0.5), vec3(-0.5,  0.5, -0.5), vec3(-0.5, -0.5, -0.5)
);



layout(location = 0) out vec3 outPosition;

layout(binding = 0) uniform sphericalMap {
    mat4 transform;
    uint layerIndex;
};

void main() {
    gl_Layer = int(layerIndex);
    vec3 position = positions[gl_VertexIndex];
    gl_Position = transform * vec4(position, 1.0);
    outPosition = position;
}
