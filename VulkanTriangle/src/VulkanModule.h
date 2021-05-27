#pragma once

#include "VulkanEngine.h"

class VulkanModule {
public:
	static VulkanModule initialize() {
		VulkanModule module;
		//module.vkEngine = vkEngine;
		return module;
	}
private:
	//VulkanEngine vkEngine;
};