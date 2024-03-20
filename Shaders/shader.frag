#version 450

layout(location = 0) in vec4 verPos;

layout(location = 0) out vec4 outColor;

layout(push_constant) uniform constants {
    mat4 mvp_mat;
    mat4 imvp_mat;
} p_const;

void main() {
    //outColor = vec4(fragColor, 1.0);
    //vec4 screencoord = vec4(gl_FragCoord.x/1280 * 2 - 1, gl_FragCoord.y/720 * 2 - 1, 0, 1);
    //vec4 modelcoord = p_const.MVP * screencoord;
    //vec4 modelpos = p_const.m_mat * verPos;
    //vec3 colors = vec3(mod(verPos.x, 0.25)*4, mod(verPos.y, 0.25)*4, mod(verPos.z, 0.25)*4);
    //outColor = vec4(colors, 1);

    vec4 fragCoordNorm = vec4(gl_FragCoord.x/1280 * 2 - 1, gl_FragCoord.x/720 * 2 - 1, gl_FragCoord.z, 1);
    vec3 verDir = vec3(2,2,2) - vec3(p_const.imvp_mat * verPos);

    vec3 curPos = vec3(verPos);
    ivec3 curCell = ivec3(floor(verPos.x), floor(verPos.y), floor(verPos.z));

    vec3 dist = mod(curPos, 1.0);
    if (verDir.x > 0) dist.x = 1-dist.x;
    if (verDir.y > 0) dist.y = 1-dist.y;
    if (verDir.z > 0) dist.z = 1-dist.z;

    if (dist.x == 0) dist.x = 1.0;
    if (dist.y == 0) dist.y = 1.0;
    if (dist.z == 0) dist.z = 1.0;

    dist /= abs(verDir);

    //for (int i = 0; i < 10; i++) {
    //
    //}
    //if (dist.x < dist.y && dist.x < dist.z)
    //    outColor = vec4(1, 0, 0, 1);
    //else if (dist.y < dist.x && dist.y < dist.z)
    //    outColor = vec4(0, 1, 0, 1);
    //else
    //    outColor = vec4(0, 0, 1, 1);
    outColor = vec4(verDir, 1);
}