//
// Created by loochis on 2/3/24.
//

#ifndef VOXCHIS_VKCMANAGER_H
#define VOXCHIS_VKCMANAGER_H

// VKChis includes
#include <memory>
#include "Utils/VKCEnumerations.h"
#include "WINChisInstance.h"
#include "VKCInstance.h"
#include "VKCSurface.h"

namespace VKChis {

    class VKCManager {
    private:
        vkc_InitFlags flags;
        std::shared_ptr<WINChisInstance> window;
        std::unique_ptr<VKCInstance> instance;
        std::unique_ptr<VKCSurface> surface;
    public:
        VKCManager(std::shared_ptr<WINChisInstance> in_window, vkc_InitFlags in_flags);
        ~VKCManager();

    };

} // VKChis

#endif //VOXCHIS_VKCMANAGER_H
