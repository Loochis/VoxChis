//
// Created by loochis on 6/11/24.
//

#include "VKCUserInterface.h"

VKCUserInterface::VKCUserInterface(shared_ptr<VKCDevice> &in_device, shared_ptr<VKCInstance> &in_instance,
                                   shared_ptr<VKCSwapChain> &in_swapchain, shared_ptr<VKCSurface> &in_surface,
                                   shared_ptr<WINChisInstance> &in_window, shared_ptr<VKCDescriptorManager> &in_descriptorManager)
  : device(in_device),
    instance(in_instance),
    swapchain(in_swapchain),
    surface(in_surface),
    window(in_window),
    descriptorManager(in_descriptorManager)
    {

    imgui_wd.Surface = surface->surface;
    imgui_wd.SurfaceFormat = swapchain->surfaceFormat;
    imgui_wd.PresentMode = swapchain->presentMode;
    imgui_wd.ClearEnable = false;

    ImGui_ImplVulkanH_CreateOrResizeWindow(instance->instance, device->physicalDevice, device->device, &imgui_wd, device->indices.graphicsFamily.value(), nullptr, window->width, window->height, imgui_imgcount);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForVulkan(window->window, true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = instance->instance;
    init_info.PhysicalDevice = device->physicalDevice;
    init_info.Device = device->device;
    init_info.QueueFamily = device->indices.graphicsFamily.value();
    init_info.Queue = device->graphicsQueue;
    init_info.PipelineCache = VK_NULL_HANDLE;
    init_info.DescriptorPool = descriptorManager->descriptorPool;
    init_info.RenderPass = imgui_wd.RenderPass;
    init_info.Subpass = 0;
    init_info.MinImageCount = imgui_imgcount;
    init_info.ImageCount = imgui_wd.ImageCount;
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    init_info.Allocator = nullptr;
    //init_info.CheckVkResultFn = check_vk_result;
    ImGui_ImplVulkan_Init(&init_info);

    // Set the clear value
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 0.10f);
    imgui_wd.ClearValue.color.float32[0] = clear_color.x * clear_color.w;
    imgui_wd.ClearValue.color.float32[1] = clear_color.y * clear_color.w;
    imgui_wd.ClearValue.color.float32[2] = clear_color.z * clear_color.w;
    imgui_wd.ClearValue.color.float32[3] = clear_color.w;
}

VKCUserInterface::~VKCUserInterface() {
    vkDeviceWaitIdle(device->device);
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    ImGui_ImplVulkanH_DestroyWindow(instance->instance, device->device, &imgui_wd, nullptr);
}
