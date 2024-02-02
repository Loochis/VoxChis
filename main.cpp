#include <iostream>
#include "VulkanChis/WINChisInstance.h"
#include "VulkanChis/VKChisInstance.h"

using namespace WINChis;
using namespace VKChis;

int main() {

    WINChisInstance wcInst = WINChisInstance(800, 600, "test");
    VKChisInstance vkcInst = VKChisInstance(wcInst, true);

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
