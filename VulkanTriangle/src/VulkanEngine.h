#pragma once

#include <GLFW/glfw3.h>
#include <cstdint>
#include <stdexcept>
#include <vulkan/vulkan.h>
#include <vector>
#include <optional>

#include "config/VulkanInstanceCreator.h"
#include "config/DebugMessenger.h"
#include "config/VulkanDeviceInitializer.h"
#include "config/VulkanSwapChainConfigurer.h"
#include "config/VulkanGraphicPipeline.h"
#include "config/VulkanDrawingBufferConfigurator.h"

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

class VulkanEngine {
public:
	VulkanEngine() {
		window = initWindow();
		initializeVulkan();

		VulkanDeviceInitializer::initializeDevice(*this);
		VulkanSwapChainConfigurer::createSwapChain(*this);
		VulkanSwapChainConfigurer::createImageViews(*this);
		VulkanGraphicPipeline::initialize(*this);
		VulkanDrawingBuffersConfigurator::configureDrawingBuffers(*this);
	}

	#ifdef NDEBUG
		const bool enableValidationLayers = false;
	#else
		const bool enableValidationLayers = true;
	#endif

	static SwapChainSupportDetails querySwapChainSupport(VulkanEngine& vkEngine, VkPhysicalDevice device) {
		SwapChainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, vkEngine.surface, &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, vkEngine.surface, &formatCount, nullptr);
		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, vkEngine.surface, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, vkEngine.surface, &presentModeCount, nullptr);
		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, vkEngine.surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	void mainLoop();
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

	GLFWwindow* window;
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;

	QueueFamilyIndices indices;
	VkSurfaceKHR surface;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;

	VkQueue graphicsQueue;
	VkQueue presentQueue;

	// SwapChain
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;
	
	// Drawing buffers
	std::vector<VkFramebuffer> swapChainFramebuffers;
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	// Graphics pipeline
	VkPipelineLayout pipelineLayout;
	VkRenderPass renderPass;
	VkPipeline graphicsPipeline;

	const int MAX_FRAMES_IN_FLIGHT = 2;
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;
	size_t currentFrame = 0;

private:
	const uint32_t width = 800;
	const uint32_t height = 600;
	GLFWwindow* initWindow();
	void initializeVulkan();
	void drawFrame();
	void cleanup();
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
};
