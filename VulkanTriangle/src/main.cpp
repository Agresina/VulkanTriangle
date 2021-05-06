

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

        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    VulkanEngine vkEngine;
    VkDebugUtilsMessengerEXT debugMessenger;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;
    size_t currentFrame = 0;

    void mainLoop() {
        while (!glfwWindowShouldClose(vkEngine.window)) {
            glfwPollEvents();
            drawFrame();
        }
        // Wait until the devices is idle before cleaning up
        vkDeviceWaitIdle(vkEngine.device);
    }

    void drawFrame() {
        vkWaitForFences(vkEngine.device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        vkAcquireNextImageKHR(vkEngine.device, vkEngine.swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
        // Check if a previous frame is using this image (i.e. there is its fence to wait on)
        if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
            vkWaitForFences(vkEngine.device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
        }
        // Mark the image as now being in use by this frame
        imagesInFlight[imageIndex] = inFlightFences[currentFrame];

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &vkEngine.commandBuffers[imageIndex];

        VkSemaphore signalSemaphores[] = { imageAvailableSemaphores[currentFrame] };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        vkResetFences(vkEngine.device, 1, &inFlightFences[currentFrame]);
        if (vkQueueSubmit(vkEngine.graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
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

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }


    void initVulkan() {
        createSyncObjects();
    }

    void createSyncObjects() {
        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        imagesInFlight.resize(vkEngine.swapChainImages.size(), VK_NULL_HANDLE);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(vkEngine.device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(vkEngine.device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(vkEngine.device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {

                throw std::runtime_error("failed to create semaphores for a frame!");
            }
        }
    }


    void cleanup() {
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(vkEngine.device, renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(vkEngine.device, imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(vkEngine.device, inFlightFences[i], nullptr);
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

