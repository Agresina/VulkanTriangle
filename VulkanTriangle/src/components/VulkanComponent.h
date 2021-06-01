#pragma once

#include "../VulkanEngine.h"

class VulkanComponent {
protected:
	VulkanEngine* vkEngine_;
public:
	VulkanComponent(VulkanEngine* vkEngine = nullptr) : vkEngine_(vkEngine){}
};