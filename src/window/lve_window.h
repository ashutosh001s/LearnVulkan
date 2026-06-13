//
//  lve_window.hpp
//  VulkanTutorial
//
//  Created by Ashutosh singh on 09/06/26.
//

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <expected>

template<typename V, typename E>
using Result =  std::expected<V, E>;

namespace lve {

class LveWindow {
    
public:
    LveWindow(uint32_t w , uint32_t h , const std::string & winName);
    ~LveWindow();

    LveWindow(const LveWindow &) = delete;
    LveWindow & operator=(const LveWindow &) = delete;
    
    [[nodiscard]] bool shouldClose() const { return glfwWindowShouldClose(window); }
    
private:
    const int width;
    const int height;
    std::string windowName;
    GLFWwindow * window;
    
    void initWindow();
    
};
}
