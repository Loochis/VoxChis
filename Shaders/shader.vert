#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

layout(set = 0, binding = 0) uniform CamUBO {
    mat4 view;
    mat4 proj;
} cam_ubo;

layout(set = 1, binding = 0) uniform ModUBO {
    mat4 model;
} mod_ubo;

void main() {
    //gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 0.0, 1.0);
    gl_Position = cam_ubo.proj * cam_ubo.view * mod_ubo.model * vec4(inPosition, 0.0, 1.0);
    fragColor = inColor;
}