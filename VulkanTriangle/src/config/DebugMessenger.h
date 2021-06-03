#pragma once

#include <iostream>

class VulkanEngine;
class DebugMessenger {
public:
	static void setupDebugMessenger(VulkanEngine& vkEngine);
private:
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
};