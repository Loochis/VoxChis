//
// Created by loochis on 6/7/24.
//

#include "VKCStatistics.h"

int VKCStatistics::startFrame() {
    // perform frameskips
    if (skip_frametimes < FRAME_SKIPS)
        return 0;

    // log start time
    inst_framerates_dt = high_resolution_clock::now();
    return 0;
}

int VKCStatistics::endFrame() {
    // perform frameskips
    if (skip_frametimes < FRAME_SKIPS) {
        skip_frametimes++;
        return 0;
    }

    skip_frametimes = 0;

    // compute delta time
    long dt_uS = duration_cast<microseconds>(high_resolution_clock::now() - inst_framerates_dt).count();

    // modify rolled average
    roll_frametime -= (double)inst_frametimes[0] / DATA_COUNT;
    roll_frametime += (double)dt_uS / DATA_COUNT;

    // scroll the buffer
    for (int i = 0; i < DATA_COUNT - 1; i++)
        inst_frametimes[i] = inst_frametimes[i+1];
    inst_frametimes[DATA_COUNT - 1] = (float)dt_uS;

    return 0;
}

int VKCStatistics::startGpuDraw() {
    return 0;
}

int VKCStatistics::endGpuDraw() {
    return 0;
}

int VKCStatistics::startCpuDraw() {
    return 0;
}

int VKCStatistics::endCpuDraw() {
    return 0;
}

int VKCStatistics::startCpuOther() {
    return 0;
}

int VKCStatistics::endCpuOther() {
    return 0;
}
