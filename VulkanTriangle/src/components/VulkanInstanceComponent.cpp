#include "VulkanInstanceComponent.h"
#include "../VulkanEngine.h"

void VulkanInstanceComponent::DoA() {
    std::cout << "Component 1 does A.\n";
    this->vkEngine_->Notify(this, "A");
}