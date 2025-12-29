/**
 * @file helper.hpp
 * @brief Utility functions for file I/O and common operations
 */

#include <filesystem>
#include <fstream>
#include <vector>

/**
 * @namespace Helper
 * @brief General utility functions used across the application
 */
namespace Helper
{
    /**
     * @brief Read entire file into memory as bytes
     * @param filename Path to file (relative or absolute)
     * @return Vector of bytes containing file contents
     * @throws std::runtime_error if file doesn't exist or can't be opened
     * @details Reads binary files (e.g., SPIR-V shaders) into memory.
     *          Prints current working directory for debugging path issues.
     */
    static std::vector<std::byte> readFile(const std::filesystem::path &filename)
    {

        std::filesystem::path exePath = std::filesystem::current_path();
        std::printf("Current path: %s\n", exePath.string().c_str());

        if (!std::filesystem::exists(filename))
        {
            throw std::runtime_error("file does not exist!");
        }

        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open())
        {
            throw std::runtime_error("failed to open file!");
        }

        std::vector<std::byte> buffer(std::filesystem::file_size(filename));

        file.seekg(0);
        file.read(reinterpret_cast<char *>(buffer.data()), buffer.size());

        return buffer;
    }
} // namespace Helper