#pragma once
#include <GLFW/glfw3.h>
#include <cstdint>
#include "VulkanEngine.h"
#include "VulkanInstanceCreator.h"
#include "DebugMessenger.h"
#include "VulkanDeviceInitializer.h"

const uint32_t DEFAULT_WIDTH = 800;
const uint32_t DEFAULT_HEIGHT = 600;

class VulkanInitializer {
public:
	VulkanInitializer() {
		width = DEFAULT_WIDTH;
		height = DEFAULT_HEIGHT;
	}
	VulkanInitializer(uint32_t w, uint32_t h) {
		width = w;
		height = h;
	}
	static int initialize(VulkanEngine* vkEngine);

private:
	uint32_t width;
	uint32_t height;
	GLFWwindow* initWindow();
	void initializeVulkan(VulkanEngine* vkEngine);

};