//
// Created by loochis on 1/28/24.
//

#ifndef VOXCHIS_VKCVALIDATOR_H
#define VOXCHIS_VKCVALIDATOR_H

#include <cstdint>
#include <cstring>
#include <vector>

namespace VKChis {
    const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
    };

    class VKCValidator {
    public:
        static bool checkValidationLayerSupport();
    };

} // VKChis

#endif //VOXCHIS_VKCVALIDATOR_H
