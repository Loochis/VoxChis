//
// Created by loochis on 6/12/24.
//

#include <fstream>
#include "VKCAssetLoader.h"
#include "../Headers/ColorMessages.h"

VKCAssetLoader::VKCAssetLoader(uint32_t in_flags) : flags(in_flags) {}

int VKCAssetLoader::LoadVOX(string file_path) {

    // Check if file is valid
    ifstream f(file_path.c_str());
    if (!f.good()) {
        f.close();
        return 1;
    }
    f.close();

    // load the file contents
    vector<char> f_contents = readFile(file_path);
    int f_idx = 0;

    // check if file is a vox file
    char header[] = "VOX ";
    if (matchStr(f_contents.data(), header, 4, 0)) {
        if (flags & VKC_ASSET_LOAD_DEBUG)
            print_colored("/// ASSET DEBUG /// - Non-vox header: " + file_path, YELLOW);
        return 1;
    }

    // get dimensions
    for (int i = 0; i < 3; i++)
        dimensions[i] = f_contents[32 + i*4];

    uint8_t vc_bytes[4];
    for (int i = 0; i < 4; i++)
        vc_bytes[i] = f_contents[56 + i];
    voxcount = (vc_bytes[3] << 24) | (vc_bytes[2] << 16) | (vc_bytes[1] << 8) | vc_bytes[0];

    // resize the vector
    voxel_data.resize(dimensions[0]*dimensions[1]*dimensions[2]);

    // write voxel data
    for (int i = 0; i < voxcount; i++) {
        uint8_t x_pos = f_contents[60 + i*4 + 0];
        uint8_t y_pos = f_contents[60 + i*4 + 1];
        uint8_t z_pos = f_contents[60 + i*4 + 2];
        voxel_data[XYZtoIDX(x_pos, y_pos, z_pos)] = f_contents[60 + i*4 + 3];
    }

    return 0;
}

int VKCAssetLoader::XYZtoIDX(int x, int y, int z) {
    return x + y*dimensions[0] + z*dimensions[0]*dimensions[1];
}

VKCAssetLoader::~VKCAssetLoader() {

}

vector<char> VKCAssetLoader::readFile(const string& str) {
    std::ifstream file(str, std::ios::ate | std::ios::binary);
    if (!file.good())
        return {};

    if (!file.is_open()) {
        throw std::runtime_error("ASSET: failed to open file!");
    }

    long fileSize = file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

int VKCAssetLoader::matchStr(const char *data, const char *match, int length, int startidx) {
    for (int i = 0; i < length; i++) {
        if (data[startidx + i] != match[i])
            return 1;
    }
    return 0;
}
