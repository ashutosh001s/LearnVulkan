//
//  lve_window.cpp
//  VulkanTutorial
//
//  Created by Ashutosh singh on 09/06/26.
//

#include "lve_window.h"

namespace lve {

LveWindow::LveWindow(uint32_t w , uint32_t h , const std::string & winName) : width(w), height(h), windowName(winName)
{
    initWindow();
}

LveWindow::~LveWindow()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void LveWindow::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    
    window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
}

} //namespace lve


