//
// Created by loochis on 1/28/24.
//

#ifndef VOXCHIS_VKCVALIDATOR_H
#define VOXCHIS_VKCVALIDATOR_H

#include <cstdint>
#include <cstring>
#include <vector>

namespace VKChis {
    class VKCValidator {
    public:
        static bool checkValidationLayerSupport(const std::vector<const char*>& validationLayers);
    };

} // VKChis

#endif //VOXCHIS_VKCVALIDATOR_H
