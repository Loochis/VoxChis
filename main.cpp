#include <iostream>
#include <memory>
#include "VulkanChis/WINChisInstance.h"
#include "VulkanChis/VKCManager.h"

using namespace VKChis;

int main() {

    uint32_t flags = VKC_DISPLAY_GPU_INFO | VKC_ENABLE_VALIDATION_LAYER;

    std::shared_ptr<WINChisInstance> wcInst = std::make_shared<WINChisInstance>(flags, 800, 600, "test");
    VKCManager vkcMan = VKCManager(wcInst, flags);

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
