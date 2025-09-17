#pragma once

///Core
#include "window.h"
#include "device.h"
#include "descriptors.h"
#include "objects.h"
#include "renderer.h"

///std
#include <exception>
#include <iostream>
#include <array>
#include <chrono>
#include <filesystem>

namespace pme
{
    struct GlobalUbo
    {
        glm::mat4 projectionView{1.f};
        glm::vec3 lightDirection = glm::normalize(glm::vec3{1.f, -3.f, -1.f});
    };

    class App
    {

        public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;
        
    
        public:
        App();
        ~App();

        App(const App &) = delete;
        App &operator=(const App &) = delete;

        void Run();

    private:
        void LoadObjects();

    private:
        Window window;
        PmeDevice device;
        PmeRenderer renderer;
        std::vector<PmeObject> pmeObjects;
        std::unique_ptr<PmeDescriptorPool> globalPool;

    };

}