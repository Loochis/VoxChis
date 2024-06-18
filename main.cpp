#include <iostream>
#include <memory>
#include "VulkanChis/VKCVulkanComponents/WINChisInstance.h"
#include "VulkanChis/VKCManager.h"
#include <GLFW/glfw3.h>

using namespace VKChis;

int main() {

    uint32_t flags = VKC_DISPLAY_GPU_INFO | VKC_ENABLE_VALIDATION_LAYER | VKC_ASSET_LOAD_DEBUG;

    std::shared_ptr<WINChisInstance> wcInst = std::make_shared<WINChisInstance>(flags, 1920, 1080, "test");
    VKCManager vkcMan = VKCManager(wcInst, flags);

    int ctr = 0;
    int *t,*b,*l,*r;
    // Main loop
    while (!glfwWindowShouldClose(wcInst->window)) {
        glfwPollEvents();
        vkcMan.DrawFrame();
        //ctr++;
        if (ctr == 10) {
            glfwGetFramebufferSize(wcInst->window, l,t);
            const char* desc;
            int error = glfwGetError(&desc);
            printf("GLFW error: %d, %s\n", error, desc);
            std::cout << l << "\n";
            ctr = 0;
        }
    }

    return 0;
}
