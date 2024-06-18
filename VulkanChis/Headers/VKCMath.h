//
// Created by loochis on 6/18/24.
//

#ifndef VOXCHIS_VKCMATH_H
#define VOXCHIS_VKCMATH_H

float lerp(float a, float b, float t) {
    return a*(1.0f-t) + b*t;
}

float lerp01(float a, float b, float t) {
    if (t > 1.0f) t = 1.0f;
    if (t < 0.0f) t = 0.0f;
    return a*(1.0f-t) + b*t;
}

#endif //VOXCHIS_VKCMATH_H
