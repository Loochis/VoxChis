#include <iostream>
#include <memory>
#include "VulkanChis/VKCDynamicComponents/WINChisInstance.h"
#include "VulkanChis/VKCDynamicComponents/VKCManager.h"
#include <GLFW/glfw3.h>

using namespace VKChis;

int main() {

    uint32_t flags = VKC_DISPLAY_GPU_INFO | VKC_ENABLE_VALIDATION_LAYER;

    std::shared_ptr<WINChisInstance> wcInst = std::make_shared<WINChisInstance>(flags, 1280, 720, "test");
    VKCManager vkcMan = VKCManager(wcInst, flags);

    // Main loop
    while (!glfwWindowShouldClose(wcInst->window)) {
        glfwPollEvents();
        vkcMan.DrawFrame();
    }

    return 0;
}
