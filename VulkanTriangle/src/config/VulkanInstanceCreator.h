#pragma once

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>
#include <iostream>

#include "../VulkanEngine.h"
#include "CreatorInfoFactory.h"

class VulkanInstanceCreator {
public:
	static VkInstance createInstance(VulkanEngine& vkEngine);
private:
	static std::vector<const char*> getRequiredExtensions(VulkanEngine& vkEngine);
	static bool checkValidationLayerSupport();
	static void setupValidationLayers(VulkanEngine& vkEngine, VkInstanceCreateInfo createInfo);
};
