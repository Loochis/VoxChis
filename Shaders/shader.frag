#version 450

layout(location = 0) in vec4 verPos;

layout(location = 0) out vec4 outColor;

layout(push_constant) uniform constants {
    mat4 mvp_mat;
    mat4 m_mat;
} p_const;

void main() {
    //outColor = vec4(fragColor, 1.0);
    //vec4 screencoord = vec4(gl_FragCoord.x/1280 * 2 - 1, gl_FragCoord.y/720 * 2 - 1, 0, 1);
    //vec4 modelcoord = p_const.MVP * screencoord;
    //vec4 modelpos = p_const.m_mat * verPos;
    vec3 colors = vec3(mod(verPos.x, 0.1)*10, mod(verPos.y, 0.1)*10, mod(verPos.z, 0.1)*10);
    outColor = vec4(colors, 1);

    //outColor = vec4(fCoord[0]/1280, fCoord[1]/720, 0, 1);
}