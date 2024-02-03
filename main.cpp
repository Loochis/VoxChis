#include <iostream>
#include <memory>
#include "VulkanChis/WINChisInstance.h"
#include "VulkanChis/VKCManager.h"

using namespace VKChis;

int main() {

    std::shared_ptr<WINChisInstance> wcInst = std::make_shared<WINChisInstance>(800, 600, "test");

    VKCManager vkcMan = VKCManager(wcInst, VALIDATION_LAYER);

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
