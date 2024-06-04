#version 450

layout(location = 0) in vec3 inPosition;

layout(location = 0) out vec4 verPos;
layout(location = 1) out vec3 verDir;

layout(push_constant) uniform constants {
    mat4 m_mat;
    vec3 im_campos;
} p_const;

layout(set = 0, binding = 0) uniform CameraUBOObject {
    mat4 vp_mat;
} camUBO;

void main() {
    //gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 0.0, 1.0);

    gl_Position = camUBO.vp_mat * p_const.m_mat * vec4(inPosition, 1.0) + gl_InstanceIndex * vec4(-5, 0, 0, 0);
    verPos = vec4(inPosition, 1.0);

    verDir = p_const.im_campos - vec3(verPos);
}