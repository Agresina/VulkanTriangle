#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <optional>
#include <set>
#include <vulkan/vulkan.h>
#include <vector>

#include "../Utils.h"
#include "../VulkanEngine.h"

class VulkanDeviceInitializer {

public:
	static void initializeDevice(VulkanEngine& vkEngine);
private:
	static void createSurface(VulkanEngine& vkEngine);
	static void pickPhysicalDevice(VulkanEngine& vkEngine);
	static void createLogicalDevice(VulkanEngine& vkEngine);
	static bool isDeviceSuitable(VulkanEngine& vkEngine, VkPhysicalDevice device);
	static bool checkDeviceExtensionSupport(VkPhysicalDevice device);
};