#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

layout(push_constant) uniform constants {
    mat4 MVP;
} p_const;

void main() {
    //gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 0.0, 1.0);
    gl_Position = p_const.MVP * vec4(inPosition, 0.0, 1.0);
    fragColor = inColor;
}