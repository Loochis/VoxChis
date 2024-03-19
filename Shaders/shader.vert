#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec4 verPos;

layout(push_constant) uniform constants {
    mat4 mvp_mat;
    mat4 m_mat;
} p_const;

void main() {
    //gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 0.0, 1.0);
    gl_Position = p_const.mvp_mat * vec4(inPosition, 0.0, 1.0);
    verPos = vec4(inPosition, 0.05, 1.0);
}