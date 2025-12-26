#include <filesystem>
#include <fstream>
#include <vector>

namespace Helper
{
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