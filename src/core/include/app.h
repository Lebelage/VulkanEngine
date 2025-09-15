#pragma once

///Core
#include "window.h"
#include "device.h"

///std
#include <exception>
#include <iostream>
#include <array>
#include <chrono>

namespace pme
{

    class App
    {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

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

    };

}