//
// Created by loochis on 2/6/24.
//

#ifndef VOXCHIS_VKCSHADERMODULE_H
#define VOXCHIS_VKCSHADERMODULE_H

#include <string>
#include <vulkan/vulkan.h>
#include <vector>

#include <iostream>
#include <utility>
#include <fstream>
#include <openssl/sha.h>

using namespace std;

namespace VKChis {

    class VKCShaderModule {
    public:
        VKCShaderModule(uint32_t in_flags, string in_file_path, VkDevice in_device, VkResult &result);
        ~VKCShaderModule();

        string comp_visname;

    private:
        uint32_t flags;

        string file_path;
        string comp_path;
        string comp_name;
        string dat_path;

        VkDevice device;
        VkShaderModule shaderModule;

        unsigned char hash[SHA_DIGEST_LENGTH];

        bool checkDat();
        VkResult compile();

        static vector<char> readFile(const string& str);
    };

} // VKChis

#endif //VOXCHIS_VKCSHADERMODULE_H
