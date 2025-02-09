#version 450

// 큐브 정점 데이터 (삼각형 리스트)
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

layout(location = 0) out vec3 viewDirection;

layout(binding = 0) uniform CameraInfo {
    mat4 proj;
    mat4 view;
};

void main() {
    viewDirection = positions[gl_VertexIndex];
    mat4 rotView = mat4(mat3(view));
    vec4 clipPos = proj * rotView * vec4(viewDirection, 1.0);
    gl_Position = clipPos.xyww;
}
