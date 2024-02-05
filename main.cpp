#include <iostream>
#include <memory>
#include "VulkanChis/WINChisInstance.h"
#include "VulkanChis/VKCManager.h"

using namespace VKChis;

int main() {

    std::shared_ptr<WINChisInstance> wcInst = std::make_shared<WINChisInstance>(800, 600, "test");

    uint32_t flags = VKC_DISPLAY_GPU_INFO | VKC_ENABLE_VALIDATION_LAYER;
    VKCManager vkcMan = VKCManager(wcInst, flags);

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
