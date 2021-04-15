#include "VulkanDeviceInitializer.h"

void VulkanDeviceInitializer::initializeDevice(VulkanEngine& vkEngine) {
	createSurface(vkEngine);
	pickPhysicalDevice(vkEngine);
	createLogicalDevice(vkEngine);
}

void VulkanDeviceInitializer::createSurface(VulkanEngine& vkEngine) {
	if (glfwCreateWindowSurface(vkEngine.instance, vkEngine.window, nullptr, &vkEngine.surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}
}

void VulkanDeviceInitializer::pickPhysicalDevice(VulkanEngine& vkEngine) {
    // Query number of graphic cards
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(vkEngine.instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    // Store all the devices in an array
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(vkEngine.instance, &deviceCount, devices.data());

    // Evaluate the devices
    for (const auto& device : devices) {
        if (isDeviceSuitable(vkEngine, device)) {
            vkEngine.physicalDevice = device;
            break;
        }
    }

    if (vkEngine.physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

void VulkanDeviceInitializer::createLogicalDevice(VulkanEngine& vkEngine) {
    // Find queue families of the picked physical device
    QueueFamilyIndices indices = Utils::findQueueFamilies(vkEngine, vkEngine.physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    // Logical device features
    VkPhysicalDeviceFeatures deviceFeatures{};

    // Create logical device struct
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (vkEngine.enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(vkEngine.physicalDevice, &createInfo, nullptr, &vkEngine.device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(vkEngine.device, indices.graphicsFamily.value(), 0, &vkEngine.graphicsQueue);
    vkGetDeviceQueue(vkEngine.device, indices.presentFamily.value(), 0, &vkEngine.presentQueue);
}

/**
    * Evaluate if a devices is suitable for the operations we want to perform
    **/
bool VulkanDeviceInitializer::isDeviceSuitable(VulkanEngine& vkEngine, VkPhysicalDevice device) {
    QueueFamilyIndices indices = Utils::findQueueFamilies(vkEngine, device);

    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = VulkanEngine::querySwapChainSupport(vkEngine, device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

bool VulkanDeviceInitializer::checkDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}
