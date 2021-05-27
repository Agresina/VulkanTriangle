#pragma once

#include <stdexcept>

//#include "../Utils.h"
#include "../VulkanEngine.h"

class VulkanEngine;
class VulkanDrawingBuffersConfigurator {
public:
	static void configureDrawingBuffers(VulkanEngine& vkEngine);
private:
	static void createFramebuffers(VulkanEngine& vkEngine);
	static void createCommandPool(VulkanEngine& vkEngine);
	static void createCommandBuffers(VulkanEngine& vkEngine);
	static void createSyncObjects(VulkanEngine& vkEngine);
};