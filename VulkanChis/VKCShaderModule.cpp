//
// Created by loochis on 2/6/24.
//


#include "VKCShaderModule.h"
#include "Utils/VKCEnumerations.h"
#include "Utils/ColorMessages.h"

namespace VKChis {
    VKCShaderModule::VKCShaderModule(uint32_t in_flags, string in_file_path, VkDevice in_device, VkResult &result)
    : flags(in_flags),
      file_path(std::move(in_file_path)),
      device(in_device)
    {
        // Check if file is valid
        ifstream f(file_path.c_str());
        if (!f.good()) {
            result = VK_ERROR_INVALID_EXTERNAL_HANDLE;
            f.close();
            return;
        }
        f.close();

        // extract substrings
        string file_dir = file_path.substr(0,file_path.find_last_of("/\\") + 1);

        unsigned long f_name_start = file_path.find_last_of("/\\") + 1;
        string file_name = file_path.substr(f_name_start, file_path.find_last_of('.') - f_name_start);

        string file_ext = file_path.substr(file_path.find_last_of('.') + 1);

        comp_name = file_name + "_" + file_ext;

        comp_visname = comp_name + ".spv";
        comp_path = file_dir + comp_visname;
        dat_path = file_dir + comp_name + ".spv";

        // Load shader file
        vector<char> f_contents = readFile(file_path);
        string f_contents_str(f_contents.begin(), f_contents.end());

        // Get hash of latest file
        SHA1(reinterpret_cast<const unsigned char *>(f_contents_str.c_str()), f_contents.size() - 1, hash);

        // Check if file has been modified / DAT file doesn't exist
        if (checkDat()) {
            if (flags & VKC_ENABLE_VALIDATION_LAYER)
                print_colored("/// INFO /// - " + comp_name + ".spv out of date, recompiling", WHITE);

            // Compile
            result = compile();
            if (result) return;

            // Write metadata
            ofstream dat_file(file_dir + comp_name + ".dat", std::ofstream::out | std::ofstream::trunc);
            dat_file << hash;
        }

        auto shaderCode = readFile(comp_path);

        // make create info from shader binary
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = shaderCode.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

        result = vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule);
    }

    bool VKCShaderModule::checkDat() {
        // Check if DAT exists
        ifstream f(dat_path);
        if (!f.good()) {
            f.close();
            return true;
        } else {

            unsigned char hash_comp[SHA_DIGEST_LENGTH];
            f.read(reinterpret_cast<char *>(hash_comp), SHA_DIGEST_LENGTH);

            // Compare the hashes
            for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
                if (hash_comp[i] != hash[i]) return true;
        }
        f.close();

        return false;
    }

    VkResult VKCShaderModule::compile() {
        // Kind of hacky way to do this
        return (VkResult)system(("glslc " + file_path + " -o " + comp_path).c_str());
        //return VK_SUCCESS;
    }

    vector<char> VKCShaderModule::readFile(const string& str) {
        std::ifstream file(str, std::ios::ate | std::ios::binary);
        if (!file.good())
            return {};

        if (!file.is_open()) {
            throw std::runtime_error("SHADER: failed to open file!");
        }

        long fileSize = file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }

    VKCShaderModule::~VKCShaderModule() {
        bool enableValidation = flags & VKC_ENABLE_VALIDATION_LAYER;

        vkDestroyShaderModule(device, shaderModule, nullptr);
        if (enableValidation) print_colored("/// CLEAN /// - Unloaded " + comp_visname, WHITE);
    }
} // VKChis