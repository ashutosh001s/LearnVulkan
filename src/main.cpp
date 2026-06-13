
#include "first_app.h"
#include <expected>
#include <print>
#include <iostream>
int main() {
    lve::FirstApp app{};
    
    
    try {
        app.run();
    } catch (const std::exception & e) {
        std::print("Error : {} ", e.what());
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
 
