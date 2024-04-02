#version 450

layout(location = 0) in vec4 verPos;
layout(location = 1) in vec3 verDir;

layout(location = 0) out vec4 outColor;

layout(push_constant) uniform constants {
    mat4 mvp_mat;
    mat4 imvp_mat;
} p_const;

void main() {

    vec3 curPos = vec3(verPos*4.0);
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
    else if (dists.x == 1) dists.x = 0;
    if (dists.y == 0) dists.y = 1;
    else if (dists.y == 1) dists.y = 0;
    if (dists.z == 0) dists.z = 1;
    else if (dists.z == 1) dists.z = 0;

    vec3 t_val_cur = dists / abs(verDir);


    outColor = vec4(0, 0, 0, 0);

    for (int i = 0; i < 40; i++) {
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

        if (length(curCell) < 1.5) {
            outColor = inverse(p_const.imvp_mat) * tempOutput;
            outColor.w = 1;
            break;
        }
    }

    //outColor = vec4(verDir, 1);

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