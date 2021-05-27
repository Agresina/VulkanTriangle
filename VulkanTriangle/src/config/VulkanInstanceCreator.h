#pragma once

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>
#include <iostream>

class VulkanInstanceCreator {
public:
	static VkInstance createInstance(bool enableValidationLayers, std::vector<const char*> validationLayers);
private:
	static std::vector<const char*> getRequiredExtensions(bool enableValidationLayers);
	static bool checkValidationLayerSupport(std::vector<const char*> validationLayers);
	static void setupValidationLayers(bool enableValidationLayers, VkInstanceCreateInfo createInfo, std::vector<const char*> validationLayers);
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
};
