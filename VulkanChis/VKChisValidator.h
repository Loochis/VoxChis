//
// Created by loochis on 1/28/24.
//

#ifndef VOXCHIS_VKCHISVALIDATOR_H
#define VOXCHIS_VKCHISVALIDATOR_H

#include <cstdint>
#include <cstring>
#include <vector>

namespace VKChis {
    const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
    };

    class VKChisValidator {
    public:
        static bool checkValidationLayerSupport();
    };

} // VKChis

#endif //VOXCHIS_VKCHISVALIDATOR_H
