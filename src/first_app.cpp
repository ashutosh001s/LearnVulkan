//
//  first_app.cpp
//  VulkanTutorial
//
//  Created by Ashutosh singh on 10/06/26.
//

#include "first_app.h"

namespace lve {

void FirstApp::run()
{
    while (!lveWindow.shouldClose()) {
        glfwPollEvents();
    }
}

}
