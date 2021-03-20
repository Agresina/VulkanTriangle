#include "VulkanInitializer.h"

int VulkanInitializer::initialize(VulkanEngine* vkEngine) {
    VulkanInitializer vkInitializer;

    vkEngine->window = vkInitializer.initWindow();
    initializeVulkan(vkEngine);

    return 0;
}

GLFWwindow* VulkanInitializer::initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    return glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);
}

void initializeVulkan(VulkanEngine* vkEngine) {
    vkEngine->instance = VulkanInstanceCreator::createInstance();
}

