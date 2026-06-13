//
// Created by Ashutosh singh on 11/06/26.
//
#pragma once
#include <expected>
#include <filesystem>
#include <vector>

namespace lve {
class LvePipeline {
public:
    LvePipeline(std::filesystem::path const &vertPath, std::filesystem::path const &fragPath);

private:
    static auto readFile(std::filesystem::path const & filePath ) -> std::expected<std::vector<char>, std::string>;

    void createGraphicsPipline(std::filesystem::path const &vertPath, std::filesystem::path const &fragPath);

    };
} // lve


