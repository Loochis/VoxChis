//
// Created by loochis on 6/12/24.
//

#ifndef VOXCHIS_VKCASSETLOADER_H
#define VOXCHIS_VKCASSETLOADER_H

#include <string>
#include <cstdint>
#include <vector>
#include "../Headers/VKCEnumerations.h"

using namespace std;

class VKCAssetLoader {
public:
    uint8_t dimensions[3];
    int voxcount;
    vector<uint8_t> voxel_data;

    VKCAssetLoader(uint32_t in_flags);
    ~VKCAssetLoader();
    int LoadVOX(string file_path);

private:
    uint32_t flags;
    char* voxFile_data;

    static vector<char> readFile(const string& str);
    static int matchStr(const char* data, const char* match, int length, int startidx);
    int XYZtoIDX(int x, int y, int z);
};


#endif //VOXCHIS_VKCASSETLOADER_H
