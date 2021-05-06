

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
        mainLoop();
        cleanup();
    }

private:
    VulkanEngine vkEngine;
    VkDebugUtilsMessengerEXT debugMessenger;

    void mainLoop() {
        while (!glfwWindowShouldClose(vkEngine.window)) {
            glfwPollEvents();
            drawFrame();
        }
        // Wait until the devices is idle before cleaning up
        vkDeviceWaitIdle(vkEngine.device);
    }

    void drawFrame() {
        vkWaitForFences(vkEngine.device, 1, &vkEngine.inFlightFences[vkEngine.currentFrame], VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        vkAcquireNextImageKHR(vkEngine.device, vkEngine.swapChain, UINT64_MAX, vkEngine.imageAvailableSemaphores[vkEngine.currentFrame], VK_NULL_HANDLE, &imageIndex);
        // Check if a previous frame is using this image (i.e. there is its fence to wait on)
        if (vkEngine.imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
            vkWaitForFences(vkEngine.device, 1, &vkEngine.imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
        }
        // Mark the image as now being in use by this frame
        vkEngine.imagesInFlight[imageIndex] = vkEngine.inFlightFences[vkEngine.currentFrame];

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { vkEngine.imageAvailableSemaphores[vkEngine.currentFrame] };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &vkEngine.commandBuffers[imageIndex];

        VkSemaphore signalSemaphores[] = { vkEngine.imageAvailableSemaphores[vkEngine.currentFrame] };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        vkResetFences(vkEngine.device, 1, &vkEngine.inFlightFences[vkEngine.currentFrame]);
        if (vkQueueSubmit(vkEngine.graphicsQueue, 1, &submitInfo, vkEngine.inFlightFences[vkEngine.currentFrame]) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;
        VkSwapchainKHR swapChains[] = { vkEngine.swapChain };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;

        vkQueuePresentKHR(vkEngine.presentQueue, &presentInfo);

        vkQueueWaitIdle(vkEngine.presentQueue);

        vkEngine.currentFrame = (vkEngine.currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

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

