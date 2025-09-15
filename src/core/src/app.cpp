#include "app.h"

pme::App::App() : window{WIDTH, HEIGHT, "PhysicsModelingEngine"}, device{window}
{
    try
    {
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
    while (!window.ShouldClose())
    {
        glfwPollEvents();
    }
}

void pme::App::LoadObjects()
{
    
}