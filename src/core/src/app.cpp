#include "app.h"

#include "swapchain.h"
#include "model.h"
#include "rendersystems/baserendersystem.h"
#include "input/keyboard_input.h"

#include "e_internalfileprovider.h"

pme::App::App() : window{WIDTH, HEIGHT, "PhysicsModelingEngine"}, device{window}, renderer{window, device}
{
    try
    {
        globalPool = PmeDescriptorPool::Builder(device)
                         .SetMaxSets(PmeSwapChain::MAX_FRAMES_IN_FLIGHT)
                         .AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, PmeSwapChain::MAX_FRAMES_IN_FLIGHT)
                         .Build();
        LoadObjects();
    }
    catch (std::runtime_error ex)
    {
    }
}

pme::App::~App()
{
}

void pme::App::Run()
{
    std::vector<std::unique_ptr<PmeBuffer>> uboBuffers(PmeSwapChain::MAX_FRAMES_IN_FLIGHT);

    for (int i = 0; i < uboBuffers.size(); i++)
    {
        uboBuffers[i] = std::make_unique<PmeBuffer>(
            device,
            sizeof(GlobalUbo),
            1,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        uboBuffers[i]->Map();
    }

    auto globalSetLayout = PmeDescriptorSetLayout::Builder(device)
                               .AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
                               .Build();

    std::vector<VkDescriptorSet> globalDescriptorsSets(PmeSwapChain::MAX_FRAMES_IN_FLIGHT);

    for (int i = 0; i < globalDescriptorsSets.size(); i++)
    {
        auto bufferInfo = uboBuffers[i]->DescriptorInfo();
        PmeDescriptorWriter(*globalSetLayout, *globalPool)
            .WriteBuffer(0, &bufferInfo)
            .Build(globalDescriptorsSets[i]);
    }

    RenderSystem renderSystem{device, renderer.GetRenderPass(), globalSetLayout->GetDescriptorSetLayout()};

    PmeCamera camera{};
    auto viewerObject = PmeObject::CreateObject();

    KeyboardInput cameraController{};
    auto currentTime = std::chrono::high_resolution_clock::now();

    while (!window.ShouldClose())
    {
        glfwPollEvents();
        
        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;

        cameraController.MoveInPlaneXZ(window.GetGLFWwindow(), frameTime, viewerObject);
        camera.SetViewXYZ(viewerObject.transform.translation, viewerObject.transform.rotation);
        float aspect = renderer.GetAspectRatio();

        camera.SetPerspectiveProjection(glm::radians(100.f), aspect, 0.1f, 10.f);

        if (auto commandBuffer = renderer.BeginFrame())
        {
            int frameIndex = renderer.GetFrameIndex();
            FrameInfo frameInfo{frameIndex, frameTime, commandBuffer, camera, globalDescriptorsSets[frameIndex]};

            GlobalUbo ubo{};
            ubo.projectionView = camera.GetProjectionMatrix() * camera.GetViewMatrix();
            uboBuffers[frameIndex]->WriteToBuffer(&ubo);
            uboBuffers[frameIndex]->Flush();

            renderer.BeginSwapChainRenderPass(commandBuffer);
            renderSystem.RenderObjects(frameInfo, pmeObjects);
            renderer.EndSwapChainRenderPass(commandBuffer);
            renderer.EndFrame();
        }
    }
}

void pme::App::LoadObjects()
{
    try
    {
        std::string vaseModelPath = FileProvider::GetModelFile("smooth_vase.obj");
        std::shared_ptr<PmeModel> vaseModel = PmeModel::CreateModelFromFile(device, vaseModelPath);
        auto cube = PmeObject::CreateObject();
        cube.model = vaseModel;
        cube.transform.translation = {.0f, .1f, 0.0f};
        cube.transform.scale = {.5f, .5f, .5f};
        pmeObjects.push_back(std::move(cube));

        std::string quadModelPath = FileProvider::GetModelFile("quad.obj");
        std::shared_ptr<PmeModel> quadModel = PmeModel::CreateModelFromFile(device, quadModelPath);
        auto quad = PmeObject::CreateObject();
        quad.model = quadModel;
        quad.transform.translation = {.0f, .1f, 0.0f};
        quad.transform.scale = {3.f, 3.5f, 1.0f};
        pmeObjects.push_back(std::move(quad));
    }
    catch (std::runtime_error ex)
    {
        Logger::Log(LogLevel::Error, ex.what());
    }
}