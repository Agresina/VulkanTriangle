#pragma once

#include "../VulkanEngine.h"
#include "../Utils.h"

class CreatorInfoFactory {
public:
	static VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo(VulkanEngine& vkEngine);

private:
};
