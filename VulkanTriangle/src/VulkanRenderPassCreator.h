#pragma once

#include <stdexcept>
#include "VulkanEngine.h"

class VulkanRenderPassCreator {
public:
	static void createRenderPass(VulkanEngine& vkEngine);
};