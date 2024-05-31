#version 450

layout(location = 0) in vec3 inPosition;

layout(location = 0) out vec4 verPos;
layout(location = 1) out vec3 verDir;

layout(push_constant) uniform constants {
    mat4 mvp_mat;
    mat4 imvp_mat;
} p_const;

void main() {
    //gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 0.0, 1.0);
    gl_Position = p_const.mvp_mat * vec4(inPosition, 1.0) + gl_InstanceIndex * vec4(-5, 0, 0, 0);
    verPos = vec4(inPosition, 1.0);

    vec3 eyePosModel = vec3(p_const.imvp_mat * vec4(0, 5, 5, 1));
    verDir = eyePosModel - vec3(verPos);
}