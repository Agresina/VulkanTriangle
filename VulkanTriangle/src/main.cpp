#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

#include <cstdint> // Necessary for UINT32_MAX
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <optional>
#include <fstream>

#include "VulkanInitializer.h"

class HelloTriangleApplication {
public:
    void run() {
        VulkanInitializer vkInitializer;
        vkInitializer.initialize(&vkEngine);
        vkEngine.mainLoop();
    }

private:
    VulkanEngine vkEngine;
};

int main() {
    HelloTriangleApplication app;

    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
