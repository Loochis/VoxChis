#version 450

layout(location = 0) in vec3 inPosition;

layout(location = 0) out vec4 verPos;

layout(push_constant) uniform constants {
    mat4 mvp_mat;
    mat4 imvp_mat;
} p_const;

void main() {
    //gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 0.0, 1.0);
    gl_Position = p_const.mvp_mat * vec4(inPosition, 1.0);
    verPos = vec4(inPosition*4.0, 1.0);
}