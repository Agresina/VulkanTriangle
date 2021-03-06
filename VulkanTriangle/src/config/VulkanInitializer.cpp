#include "VulkanInitializer.h"

int VulkanInitializer::initialize(VulkanEngine& vkEngine) {
    VulkanInitializer vkInitializer;

    vkEngine.window = vkInitializer.initWindow();
    vkInitializer.initializeVulkan(vkEngine);

    VulkanDeviceInitializer::initializeDevice(vkEngine);
    VulkanSwapChainConfigurer::createSwapChain(vkEngine);
    VulkanSwapChainConfigurer::createImageViews(vkEngine);
    VulkanGraphicPipeline::initialize(vkEngine);
    
    VulkanDrawingBuffersConfigurator::configureDrawingBuffers(vkEngine);

    return 0;
}

GLFWwindow* VulkanInitializer::initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    return glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);
}

void VulkanInitializer::initializeVulkan(VulkanEngine& vkEngine) {
    vkEngine.instance = VulkanInstanceCreator::createInstance(vkEngine.enableValidationLayers, vkEngine.validationLayers);
    DebugMessenger::setupDebugMessenger(vkEngine);
}

