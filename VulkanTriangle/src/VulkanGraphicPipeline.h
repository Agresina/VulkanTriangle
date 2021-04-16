#pragma once

#include "VulkanEngine.h"

class VulkanGraphicPipeline {
public:
	static void createGraphicsPipeline(VulkanEngine& vkEngine);
private:
	static VkShaderModule createShaderModule(VulkanEngine& vkEngine, const std::vector<char>& code);
};