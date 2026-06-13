cmake_minimum_required(VERSION 3.26)
project(LearnVulkan)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ─────────────────────────────────────────────
#  Vulkan SDK paths
# ─────────────────────────────────────────────
set(VULKAN_SDK "/Users/ashu/VulkanSDK/1.4.341.1/macOS")

set(VULKAN_INCLUDE_DIR "${VULKAN_SDK}/include")
set(VULKAN_LIB_DIR     "${VULKAN_SDK}/lib")
set(VULKAN_BIN_DIR     "${VULKAN_SDK}/bin")

# ─────────────────────────────────────────────
#  Find Vulkan (point CMake at your SDK)
# ─────────────────────────────────────────────
set(ENV{VULKAN_SDK} "${VULKAN_SDK}")
find_package(Vulkan REQUIRED)

if (NOT Vulkan_FOUND)
    message(FATAL_ERROR "Vulkan not found! Check your VULKAN_SDK path.")
endif ()
message(STATUS "Vulkan found: ${Vulkan_INCLUDE_DIRS}")

# ─────────────────────────────────────────────
#  Homebrew prefix (Apple Silicon vs Intel)
# ─────────────────────────────────────────────
if (CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
    set(HOMEBREW_PREFIX "/opt/homebrew")
else ()
    set(HOMEBREW_PREFIX "/usr/local")
endif ()

# ─────────────────────────────────────────────
#  GLFW (via Homebrew)
# ─────────────────────────────────────────────
find_package(glfw3 REQUIRED
        PATHS "${HOMEBREW_PREFIX}/lib/cmake/glfw3"
        NO_DEFAULT_PATH)

if (NOT glfw3_FOUND)
    message(FATAL_ERROR "GLFW not found! Run: brew install glfw")
endif ()
message(STATUS "GLFW found")

# ─────────────────────────────────────────────
#  GLM (header-only, via Homebrew)
# ─────────────────────────────────────────────
find_path(GLM_INCLUDE_DIR glm/glm.hpp
        PATHS "${HOMEBREW_PREFIX}/include"
        NO_DEFAULT_PATH)

if (NOT GLM_INCLUDE_DIR)
    message(FATAL_ERROR "GLM not found! Run: brew install glm")
endif ()
message(STATUS "GLM found: ${GLM_INCLUDE_DIR}")

# ─────────────────────────────────────────────
#  Source files  (add yours here)
# ─────────────────────────────────────────────
file(GLOB_RECURSE SOURCES
        "${CMAKE_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_SOURCE_DIR}/src/*.h"
)

# Fallback: if no src/ dir exists yet, use root-level main.cpp
if (NOT SOURCES)
    set(SOURCES src/main.cpp
            src/lve_pipeline.cpp
            src/lve_pipeline.h
            src/lve_device.cpp
            src/lve_device.h)
endif ()

add_executable(${PROJECT_NAME} ${SOURCES})

# ─────────────────────────────────────────────
#  Include directories
# ─────────────────────────────────────────────
target_include_directories(${PROJECT_NAME} PRIVATE
        ${VULKAN_INCLUDE_DIR}
        ${GLM_INCLUDE_DIR}
        ${HOMEBREW_PREFIX}/include        # catches any other Homebrew headers
)

# ─────────────────────────────────────────────
#  Link libraries
# ─────────────────────────────────────────────
target_link_libraries(${PROJECT_NAME} PRIVATE
        Vulkan::Vulkan
        glfw
)

# ─────────────────────────────────────────────
#  MoltenVK runtime env (macOS only)
# ─────────────────────────────────────────────
if (APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
            # Embed the rpath so the binary finds libvulkan at runtime
            BUILD_RPATH "${VULKAN_LIB_DIR}"
            INSTALL_RPATH "${VULKAN_LIB_DIR}"
    )

    # Pass the ICD and layer paths as compile definitions (optional but handy)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
            VK_ICD_FILENAMES="${VULKAN_SDK}/share/vulkan/icd.d/MoltenVK_icd.json"
    )
endif ()

# ─────────────────────────────────────────────
#  GLSL → SPIR-V shader compilation  (optional)
# ─────────────────────────────────────────────
find_program(GLSLC glslc HINTS "${VULKAN_BIN_DIR}")

if (GLSLC)
    message(STATUS "glslc found: ${GLSLC}")

    file(GLOB_RECURSE GLSL_SOURCES
            "${CMAKE_SOURCE_DIR}/shaders/*.vert"
            "${CMAKE_SOURCE_DIR}/shaders/*.frag"
            "${CMAKE_SOURCE_DIR}/shaders/*.comp"
    )
    message(STATUS "GLSL_SOURCES=${GLSL_SOURCES}")

    foreach (GLSL_FILE ${GLSL_SOURCES})
        get_filename_component(FILE_NAME ${GLSL_FILE} NAME)
        set(SPIRV_OUTPUT "${CMAKE_BINARY_DIR}/shaders/${FILE_NAME}.spv")

        add_custom_command(
                OUTPUT  ${SPIRV_OUTPUT}
                COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/shaders"
                COMMAND ${GLSLC} ${GLSL_FILE} -o ${SPIRV_OUTPUT}
                DEPENDS ${GLSL_FILE}
                COMMENT "Compiling shader: ${FILE_NAME}"
        )
        list(APPEND SPIRV_FILES ${SPIRV_OUTPUT})
    endforeach ()

    if (SPIRV_FILES)
        add_custom_target(Shaders ALL DEPENDS ${SPIRV_FILES})
        add_dependencies(${PROJECT_NAME} Shaders)
    endif ()
else ()
    message(WARNING "glslc not found – shaders will NOT be compiled automatically.")
endif ()