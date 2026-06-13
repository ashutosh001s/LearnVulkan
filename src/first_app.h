//
//  first_app.hpp
//  VulkanTutorial
//
//  Created by Ashutosh singh on 09/06/26.
//
#pragma once
#include "window/lve_window.h"
#include "lve_pipeline.h"

#include <string>

namespace lve {
class FirstApp {
public:
    static constexpr uint32_t WIDTH = 800;
    static constexpr uint32_t HEIGHT = 600;
    
    void run();
    
private:
    LveWindow lveWindow{WIDTH, HEIGHT, "Hello Vulkan"};
    LvePipeline lvePipeline {"shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv"};
    
};
}

