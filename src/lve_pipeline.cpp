//
// Created by Ashutosh singh on 11/06/26.
//

#include "lve_pipeline.h"
#include <fstream>
#include <expected>
#include <format>
#include <ostream>

namespace lve {
    LvePipeline::LvePipeline(std::filesystem::path const &vertPath, std::filesystem::path const &fragPath)
    {
        createGraphicsPipline(vertPath, fragPath);
    }

    auto LvePipeline::readFile(std::filesystem::path const &filePath) -> std::expected<std::vector<char>, std::string>
    {
        std::ifstream file{filePath, std::ios::ate | std::ios::binary};

        if (!file.is_open()) {
            const std::string msg = std::format("failed to open file {}", filePath.c_str());
            return std::unexpected(msg);
        }

        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();
        return buffer;
    }

    void LvePipeline::createGraphicsPipline(std::filesystem::path const &vertPath,
        std::filesystem::path const &fragPath)
    {
        const auto vertCode = readFile(vertPath);
        const auto fragCode = readFile(fragPath);

        std::println("Vertex shader code size : {}", vertCode.value().size());

        std::println("Fragment shader code size : {}", fragCode.value().size());

    }
} // lve