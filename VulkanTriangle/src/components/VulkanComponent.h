#pragma once

#include <iostream>
#include <string>

class VulkanEngine;
class VulkanComponent {
protected:
	VulkanEngine* vkEngine_;
public:
	VulkanComponent(VulkanEngine* vkEngine = nullptr) : vkEngine_(vkEngine){}
	void set_vulkanEngine(VulkanEngine* vkEngine) {
		this->vkEngine_ = vkEngine;
	}
};