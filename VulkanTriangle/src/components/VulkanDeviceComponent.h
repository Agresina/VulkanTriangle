#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <optional>
#include <set>
#include <vulkan/vulkan.h>
#include <vector>

#include "VulkanComponent.h"

class VulkanInstanceComponent;
class VulkanDeviceComponent : public VulkanComponent {

public:
	VulkanDeviceComponent(VulkanEngine* vkEngine, VulkanInstanceComponent* vkInstanceComponent);
	void initialize(VulkanInstanceComponent* vkInstanceComponent);
	static void initializeDevice(VulkanEngine& vkEngine);

	VkSurfaceKHR surface;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;
private:
	void createSurface(VulkanInstanceComponent* vkInstanceComponent);
	void pickPhysicalDevice();
	void createLogicalDevice();
	static bool isDeviceSuitable(VulkanEngine* vkEngine, VkPhysicalDevice device);
	static bool checkDeviceExtensionSupport(VkPhysicalDevice device);

private:
	VulkanEngine* _vkEngine;
};