#version 450

layout(location = 0) in vec3 inPosition;

layout(location = 0) out vec4 verPos;
layout(location = 1) out vec3 verDir;

layout(push_constant) uniform constants {
    mat4 m_mat;
    vec3 im_campos;
    float sp_size;
} p_const;

layout(set = 0, binding = 0) uniform CameraUBOObject {
    mat4 vp_mat;
    float sp_size;
} camUBO;

void main() {

    // TODO: Cache these in a UBO/PC?? They don't change after object instantiation!
    vec3 dimensions = vec3(20.0, 21.0, 20.0);

    //gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 0.0, 1.0);

    verPos = vec4(inPosition, 1.0);
    verDir = p_const.im_campos - inPosition*dimensions;

    gl_Position = camUBO.vp_mat * p_const.m_mat * vec4(inPosition*dimensions, 1.0);// + gl_InstanceIndex * vec4(-5, 0, 0, 0);

}