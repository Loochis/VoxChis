#version 450

layout(location = 0) in vec4 verPos;
layout(location = 1) in vec3 verDir;

layout(location = 0) out vec4 outColor;

layout(push_constant) uniform constants {
    mat4 m_mat;
    vec3 im_campos;
    float sp_size;
} p_const;

layout(binding = 1) uniform usampler3D texSampler;

void main() {

    outColor = vec4(0);
    //return;

    vec3 curPos = vec3(verPos*10.0);
    ivec3 curCell = ivec3(ceil(curPos));
    float t_total = 0;

    // compute axis deltas
    vec3 t_val_max = abs(1.0 / verDir);

    // compute initial deltas
    vec3 dists = mod(curPos, 1.0);

    if (verDir.x < 0) dists.x = 1 - dists.x;
    if (verDir.y < 0) dists.y = 1 - dists.y;
    if (verDir.z < 0) dists.z = 1 - dists.z;

    if (dists.x == 0) dists.x = 1;
    else if (dists.x >= 1) dists.x = 0;
    if (dists.y == 0) dists.y = 1;
    else if (dists.y >= 1) dists.y = 0;
    if (dists.z == 0) dists.z = 1;
    else if (dists.z >= 1) dists.z = 0;

    if (curPos.x >= 9.999) {dists.x = 0; curCell.x++; };
    if (curPos.y >= 9.999) {dists.y = 0; curCell.y++; };
    if (curPos.z >= 9.999) {dists.z = 0; curCell.z++; };

    vec3 t_val_cur = dists / abs(verDir);


    vec3 norm = vec3(0);

    // Cast the ray

    for (int i = 0; i < 100; i++) {
        vec4 tempOutput = vec4(0,0,0,0);
        if (t_val_cur.x < t_val_cur.y && t_val_cur.x < t_val_cur.z) {
            if (verDir.x < 0)   curCell.x++;
            else                curCell.x--;

            tempOutput = vec4(sign(verDir.x), 0, 0, 0);
            t_total += t_val_cur.x;

            t_val_cur.y -= t_val_cur.x;
            t_val_cur.z -= t_val_cur.x;
            t_val_cur.x = t_val_max.x;
        }
        else if (t_val_cur.y < t_val_cur.x && t_val_cur.y < t_val_cur.z) {
            if (verDir.y < 0)   curCell.y++;
            else                curCell.y--;

            tempOutput = vec4(0, sign(verDir.y), 0, 0);
            t_total += t_val_cur.y;

            t_val_cur.x -= t_val_cur.y;
            t_val_cur.z -= t_val_cur.y;
            t_val_cur.y = t_val_max.y;
        }
        else {
            if (verDir.z < 0)   curCell.z++;
            else                curCell.z--;

            tempOutput = vec4(0, 0, sign(verDir.z), 0);
            t_total += t_val_cur.z;

            t_val_cur.x -= t_val_cur.z;
            t_val_cur.y -= t_val_cur.z;
            t_val_cur.z = t_val_max.z;
        }

        if (curCell.x >=  11 || curCell.y >=  11 || curCell.z >=  11) break;
        if (curCell.x <= -10 || curCell.y <= -10 || curCell.z <= -10) break;

        if (textureLod(texSampler, curCell/10.0, 0).x >= p_const.sp_size) {
            norm = vec3(p_const.m_mat * tempOutput);
            //norm = vec3(p_const.mvp_mat * tempOutput);

            float brightness = 0.1 + clamp(dot(norm, normalize(vec3(-3, 0, 1))), 0.0, 1.0);
            outColor = vec4(brightness, brightness, brightness, 1);
            //outColor = vec4(abs(norm), 1.0);

            break;
        }
    }





    /*
    if (t_val_cur.x < t_val_cur.y && t_val_cur.x < t_val_cur.z) {
        outColor = vec4(1, 0, 0, 1);
    }
    else if (t_val_cur.y < t_val_cur.x && t_val_cur.y < t_val_cur.z) {
        outColor = vec4(0, 1, 0, 1);
    }
    else {
        outColor = vec4(0, 0, 1, 1);
    }
*/

    //outColor =

    //if (t_val_cur.x < t_val_cur.y && t_val_cur.x < t_val_cur.z)
    //    outColor = vec4(1, 0, 0, 1);
    //else if (t_val_cur.y < t_val_cur.x && t_val_cur.y < t_val_cur.z)
    //    outColor = vec4(0, 1, 0, 1);
    //else
    //    outColor = vec4(0, 0, 1, 1);

}