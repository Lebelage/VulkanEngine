#pragma once
#include <exception>
#include <iostream>
#include <array>


#include "device.h"
#include "pipeline.h"
#include "objects.h"
#include "frameinfo.h"


#include <glm/gtc/constants.hpp>

namespace pme
{
    struct SimplePushConstantData
    {
        glm::mat4 modelMatrix{1.f};
        glm::mat4 normalMatrix{1.f};
    };

    class RenderSystem
    {
    public:
        RenderSystem(PmeDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~RenderSystem();

        RenderSystem(const RenderSystem &) = delete;
        RenderSystem &operator=(const RenderSystem &) = delete;

    public:
        void RenderObjects(FrameInfo &frameInfo, std::vector<PmeObject> &gameObjects);

    private:
        void CreatePipeLineLayout(VkDescriptorSetLayout globalSetLayout);
        void CreatePipeLine(VkRenderPass renderPass);

    private:
        PmeDevice& device;
        VkPipelineLayout pipelineLayout;
        std::unique_ptr<PmePipeline> pPipeline;
    };

}