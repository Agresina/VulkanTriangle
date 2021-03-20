#pragma once

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

class VulkanEngine {
public:
	int test();
	GLFWwindow* window;
	VkInstance instance;
};
