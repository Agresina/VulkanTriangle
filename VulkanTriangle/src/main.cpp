

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

#include <cstdint> // Necessary for UINT32_MAX
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <optional>

#include <fstream>

#include "VulkanInitializer.h"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;
const int MAX_FRAMES_IN_FLIGHT = 2;

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

class HelloTriangleApplication {
public:
    void run() {
        VulkanInitializer vkInitializer;
        vkInitializer.initialize(&vkEngine);
        vkEngine.mainLoop();
    }

private:
    VulkanEngine vkEngine;
    VkDebugUtilsMessengerEXT debugMessenger;

    void cleanup() {
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(vkEngine.device, vkEngine.renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(vkEngine.device, vkEngine.imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(vkEngine.device, vkEngine.inFlightFences[i], nullptr);
        }
        vkDestroyCommandPool(vkEngine.device, vkEngine.commandPool, nullptr);
        for (auto framebuffer : vkEngine.swapChainFramebuffers) {
            vkDestroyFramebuffer(vkEngine.device, framebuffer, nullptr);
        }
        vkDestroyPipeline(vkEngine.device, vkEngine.graphicsPipeline, nullptr);
        vkDestroyPipelineLayout(vkEngine.device, vkEngine.pipelineLayout, nullptr);
        vkDestroyRenderPass(vkEngine.device, vkEngine.renderPass, nullptr);
        for (auto imageView : vkEngine.swapChainImageViews) {
            vkDestroyImageView(vkEngine.device, imageView, nullptr);
        }
        vkDestroySwapchainKHR(vkEngine.device, vkEngine.swapChain, nullptr);
        vkDestroyDevice(vkEngine.device, nullptr);

        if (enableValidationLayers) {
            DestroyDebugUtilsMessengerEXT(vkEngine.instance, debugMessenger, nullptr);
        }

        vkDestroySurfaceKHR(vkEngine.instance, vkEngine.surface, nullptr);
        vkDestroyInstance(vkEngine.instance, nullptr);

        glfwDestroyWindow(vkEngine.window);

        glfwTerminate();
    }
};

int main() {
    HelloTriangleApplication app;

    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

