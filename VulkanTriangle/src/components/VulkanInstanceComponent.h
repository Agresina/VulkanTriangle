#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "VulkanComponent.h"

class VulkanInstanceComponent : public VulkanComponent {
public:
    void DoA();
    static VulkanInstanceComponent initialize(VulkanEngine* vkEngine, bool enableValidationLayers, std::vector<const char*> validationLayers);
private:
	VkInstance vkInstance;
	static std::vector<const char*> getRequiredExtensions(bool enableValidationLayers);
	static bool checkValidationLayerSupport(std::vector<const char*> validationLayers);
	static void setupValidationLayers(bool enableValidationLayers, VkInstanceCreateInfo createInfo, std::vector<const char*> validationLayers);
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
	// Structs
	static VkApplicationInfo vkApplicationInfoStruct();
	static VkInstanceCreateInfo vkInstanceCreateInfoStruct(VkApplicationInfo* vkApplicationInfo);
};