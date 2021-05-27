#include "VulkanDrawingBufferConfigurator.h"

void VulkanDrawingBuffersConfigurator::configureDrawingBuffers(VulkanEngine& vkEngine) {
	createFramebuffers(vkEngine);
	createCommandPool(vkEngine);
	createCommandBuffers(vkEngine);
    createSyncObjects(vkEngine);
}

void VulkanDrawingBuffersConfigurator::createFramebuffers(VulkanEngine& vkEngine) {
    vkEngine.swapChainFramebuffers.resize(vkEngine.swapChainImageViews.size());

    for (size_t i = 0; i < vkEngine.swapChainImageViews.size(); i++) {
        VkImageView attachments[] = {
            vkEngine.swapChainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = vkEngine.renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = vkEngine.swapChainExtent.width;
        framebufferInfo.height = vkEngine.swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(vkEngine.device, &framebufferInfo, nullptr, &vkEngine.swapChainFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}

void VulkanDrawingBuffersConfigurator::createCommandPool(VulkanEngine& vkEngine) {
    //QueueFamilyIndices queueFamilyIndices = Utils::findQueueFamilies(vkEngine, vkEngine.physicalDevice);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = vkEngine.indices.graphicsFamily.value();
    poolInfo.flags = 0; // Optional

    if (vkCreateCommandPool(vkEngine.device, &poolInfo, nullptr, &vkEngine.commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
}

void VulkanDrawingBuffersConfigurator::createCommandBuffers(VulkanEngine& vkEngine) {
    vkEngine.commandBuffers.resize(vkEngine.swapChainFramebuffers.size());
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = vkEngine.commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)vkEngine.commandBuffers.size();

    if (vkAllocateCommandBuffers(vkEngine.device, &allocInfo, vkEngine.commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }

    for (size_t i = 0; i < vkEngine.commandBuffers.size(); i++) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0; // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional

        if (vkBeginCommandBuffer(vkEngine.commandBuffers[i], &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = vkEngine.renderPass;
        renderPassInfo.framebuffer = vkEngine.swapChainFramebuffers[i];

        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = vkEngine.swapChainExtent;

        VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(vkEngine.commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(vkEngine.commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, vkEngine.graphicsPipeline);
        vkCmdDraw(vkEngine.commandBuffers[i], 3, 1, 0, 0);
        vkCmdEndRenderPass(vkEngine.commandBuffers[i]);

        if (vkEndCommandBuffer(vkEngine.commandBuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }

    }
}

void VulkanDrawingBuffersConfigurator::createSyncObjects(VulkanEngine& vkEngine) {
    vkEngine.imageAvailableSemaphores.resize(vkEngine.MAX_FRAMES_IN_FLIGHT);
    vkEngine.renderFinishedSemaphores.resize(vkEngine.MAX_FRAMES_IN_FLIGHT);
    vkEngine.inFlightFences.resize(vkEngine.MAX_FRAMES_IN_FLIGHT);
    vkEngine.imagesInFlight.resize(vkEngine.swapChainImages.size(), VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < vkEngine.MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(vkEngine.device, &semaphoreInfo, nullptr, &vkEngine.imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(vkEngine.device, &semaphoreInfo, nullptr, &vkEngine.renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(vkEngine.device, &fenceInfo, nullptr, &vkEngine.inFlightFences[i]) != VK_SUCCESS) {

            throw std::runtime_error("failed to create semaphores for a frame!");
        }
    }
}
