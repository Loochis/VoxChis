#version 450

layout(location = 0) in vec4 verPos;
layout(location = 1) in vec3 verDir;
layout(depth_greater) out float gl_FragDepth;

layout(location = 0) out vec4 outColor;

layout(push_constant) uniform constants {
    mat4 m_mat;
    vec3 im_campos;
    float sp_size;
} p_const;

layout(binding = 1) uniform usampler3D texSampler;

void main() {
    outColor = vec4(0);

    // TODO: Cache these in a UBO/PC?? They don't change after object instantiation!
    vec3 dimensions = vec3(20.0, 21.0, 20.0);
    vec3 jumps = 1.0 / dimensions;

    vec3 startPos = vec3(verPos) + vec3(0.5);
    vec3 curPos = startPos;
    float t_total = 0;

    // compute axis deltas
    vec3 t_val_max = abs(1.0 / verDir);

    // compute initial deltas
    vec3 dists = mod(startPos*dimensions, 1.0);

    if (verDir.x < 0) dists.x = 1 - dists.x;
    if (verDir.y < 0) dists.y = 1 - dists.y;
    if (verDir.z < 0) dists.z = 1 - dists.z;

    if (dists.x == 0) dists.x = 1;
    //else if (dists.x >= 1) dists.x = 0;
    if (dists.y == 0) dists.y = 1;
    //else if (dists.y >= 1) dists.y = 0;
    if (dists.z == 0) dists.z = 1;
    //else if (dists.z >= 1) dists.z = 0;

    // I have no idea WTF that addition is, it just works for some reason
    if (curPos.x >= 0.9999) { dists.x = 0; curPos.x += jumps.x/2; }
    if (curPos.y >= 0.9999) { dists.y = 0; curPos.y += jumps.y/2; }
    if (curPos.z >= 0.9999) { dists.z = 0; curPos.z += jumps.z/2; }
    if (curPos.x <= 0.0001) { dists.x = 0; curPos.x -= jumps.x/2; }
    if (curPos.y <= 0.0001) { dists.y = 0; curPos.y -= jumps.y/2; }
    if (curPos.z <= 0.0001) { dists.z = 0; curPos.z -= jumps.z/2; }

    //dists *= jumps;

    //vec3 t_val_cur = dists / abs(verDir);
    vec3 t_val_cur = dists * t_val_max;

    vec3 norm = vec3(0);

    // Cast the ray

    for (int i = 0; i < 100; i++) {
        vec4 tempNorm;
        if (t_val_cur.x < t_val_cur.y && t_val_cur.x < t_val_cur.z) {
            if (verDir.x < 0)   curPos.x += jumps.x;
            else                curPos.x -= jumps.x;

            tempNorm = vec4(sign(verDir.x), 0, 0, 0);
            t_total += t_val_cur.x;

            t_val_cur.y -= t_val_cur.x;
            t_val_cur.z -= t_val_cur.x;
            t_val_cur.x = t_val_max.x;
        }
        else if (t_val_cur.y < t_val_cur.x && t_val_cur.y < t_val_cur.z) {
            if (verDir.y < 0)   curPos.y += jumps.y;
            else                curPos.y -= jumps.y;

            tempNorm = vec4(0, sign(verDir.y), 0, 0);
            t_total += t_val_cur.y;

            t_val_cur.x -= t_val_cur.y;
            t_val_cur.z -= t_val_cur.y;
            t_val_cur.y = t_val_max.y;
        }
        else {
            if (verDir.z < 0)   curPos.z += jumps.z;
            else                curPos.z -= jumps.z;

            tempNorm = vec4(0, 0, sign(verDir.z), 0);
            t_total += t_val_cur.z;

            t_val_cur.x -= t_val_cur.z;
            t_val_cur.y -= t_val_cur.z;
            t_val_cur.z = t_val_max.z;
        }

        if (curPos.x > 1 || curPos.y > 1 || curPos.z > 1) break;
        if (curPos.x < 0 || curPos.y < 0 || curPos.z < 0) break;

        vec3 sample_point = curPos;
        uint texVal = textureLod(texSampler, sample_point, 0).x;
        if (texVal > 0) {
            /*
            if (i == 0) {
                outColor = vec4(1,0,1,1);
                return;
            } else if (i == 1) {
                outColor = vec4(0,1,1,1);
                return;
            }
*/
            // compute normal
            norm = normalize(vec3(p_const.m_mat * tempNorm));
            //norm = vec3(p_const.mvp_mat * tempOutput);

            float brightness = 0.1 + clamp(dot(norm, normalize(vec3(-3, 0, 1))), 0.0, 1.0);
            //outColor = vec4(brightness * texVal/255.0f, brightness, brightness, 1);


            // TODO: Automatically compute near/far plane delta
            // TODO: Get scale factor in the shader
            float z = ((gl_FragCoord.z + t_total) / gl_FragCoord.w) / (10.0 - 0.1);
            //float z2 = ((gl_FragCoord.z) / gl_FragCoord.w) / (2.0 - 0.1);
            gl_FragDepth = z;
            float tmp = z;
            //outColor = vec4(tmp, tmp, tmp, 1);
            outColor = vec4(abs(tempNorm.xyz), 1.0);
            //outColor = vec4(brightness, brightness, brightness, 1);

            break;
        }
        gl_FragDepth = 1.0;
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