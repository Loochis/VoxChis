//
// Created by loochis on 6/7/24.
//

#ifndef VOXCHIS_VKCSTATISTICS_H
#define VOXCHIS_VKCSTATISTICS_H

#define DATA_COUNT 100
#define FRAME_SKIPS 10

#include <chrono>
using namespace std::chrono;

class VKCStatistics {
public:
    // STAT TRACKING
    // instantaneous values
    float inst_frametimes[DATA_COUNT];
    float inst_framerates[DATA_COUNT];
    float inst_gputimes_draw[DATA_COUNT];
    float inst_cputimes_draw[DATA_COUNT];
    float inst_cputimes_other[DATA_COUNT];

    // rolled averages
    double roll_frametime;
    double roll_framerate;
    double roll_gputime_draw;
    double roll_cputime_draw;
    double roll_cputime_other;

    // tracking vars
    time_point<system_clock> inst_frametimes_dt;
    time_point<system_clock> inst_framerates_dt;
    time_point<system_clock> inst_gputimes_draw_dt;
    time_point<system_clock> inst_cputimes_draw_dt;
    time_point<system_clock> inst_cputimes_other_dt;

    int skip_frametimes;
    int skip_framerates;

    int startFrame();
    int endFrame();

    int startGpuDraw();
    int endGpuDraw();

    int startCpuDraw();
    int endCpuDraw();

    int startCpuOther();
    int endCpuOther();
};


#endif //VOXCHIS_VKCSTATISTICS_H
