#pragma once

#include <string>
#include <filesystem>
#include <exception>

namespace pme
{

    class FileProvider
    {

    public:
        static std::string GetModelFile(std::string fileName)
        {
            std::filesystem::path current = std::filesystem::current_path().parent_path();
            std::filesystem::path filePath;
            try
            {

                filePath = current / "models";

                filePath = filePath / fileName;

                if (!std::filesystem::exists(filePath))
                {
                    throw std::runtime_error("File: " + fileName + " does not exists");
                }
            }
            catch (std::exception ex)
            {
                throw std::runtime_error(ex.what());
            }

            return filePath.string();
        };

        static std::string GetShaderFile(std::string fileName)
        {
            std::filesystem::path current = std::filesystem::current_path().parent_path();
            std::filesystem::path filePath;
            try
            {

                filePath = current / "shaders";

                filePath = filePath / fileName;

                if (!std::filesystem::exists(filePath))
                {
                    throw std::runtime_error("File: " + fileName + " does not exists");
                }
            }
            catch (std::exception ex)
            {
                throw std::runtime_error(ex.what());
            }

            return filePath.string();
        };
    };
}