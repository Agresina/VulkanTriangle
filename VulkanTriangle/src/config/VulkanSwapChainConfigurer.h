#pragma once

#include "../VulkanEngine.h"
//#include "../Utils.h"

class VulkanEngine;
//class Utils;
class VulkanSwapChainConfigurer {
public:
	static void createSwapChain(VulkanEngine& vkEngine);
	static void createImageViews(VulkanEngine& vkEngine);
private:
	static SwapChainSupportDetails querySwapChainSupport(VulkanEngine& vkEngine, VkPhysicalDevice device);
	static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	static VkExtent2D chooseSwapExtent(VulkanEngine& vkEngine, const VkSurfaceCapabilitiesKHR& capabilities);
};