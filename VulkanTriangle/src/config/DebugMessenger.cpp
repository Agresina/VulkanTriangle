#include "DebugMessenger.h"

void DebugMessenger::setupDebugMessenger(VulkanEngine& vkEngine) {
    if (!vkEngine.enableValidationLayers) return;

    std::cout << "Setting up Debug Messenger" << "\n";
    VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo = CreatorInfoFactory::debugMessengerCreateInfo(vkEngine);
    if (DebugMessenger::CreateDebugUtilsMessengerEXT(vkEngine.instance, &debugMessengerCreateInfo, nullptr, &vkEngine.debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}
VkResult DebugMessenger::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}