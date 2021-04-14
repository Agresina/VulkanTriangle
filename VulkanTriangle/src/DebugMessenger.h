#pragma once

#include <iostream>
#include "VulkanEngine.h"
#include "CreatorInfoFactory.h"

class DebugMessenger {
public:
	static void setupDebugMessenger(VulkanEngine& vkEngine);
private:
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
};