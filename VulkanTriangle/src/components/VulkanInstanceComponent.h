#pragma once

#include "VulkanComponent.h";

class VulkanInstanceComponent : public VulkanComponent {
public:
    void DoA() {
        std::cout << "Component 1 does A.\n";
        this->vkEngine_->Notify(this, "A");
    }
};