#pragma once

#include "VulkanEngine.h"

class VulkanGraphicPipeline {
public:
	static void initialize(VulkanEngine& vkEngine);
private:
	static void createGraphicsPipeline(VulkanEngine& vkEngine);
	static void createRenderPass(VulkanEngine& vkEngine);
	static VkShaderModule createShaderModule(VulkanEngine& vkEngine, const std::vector<char>& code);
};