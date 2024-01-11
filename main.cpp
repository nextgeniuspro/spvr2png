#include <iostream>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <cctype>
#include <string>
#include "SegaPVRImage.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace fs = std::filesystem;

void PrintHelp();
std::vector<std::string> TraverseDirectory(const fs::path& directory);
void ProcessFiles(const std::vector<std::string>& sourceFiles, const std::string& destPath);
int ConvertPvr2Png(const std::string& sourceFile, const std::string& destFile);
int ConvertPng2Pvr(const std::string& sourceFile, const std::string& destFile);
void tolower(std::string& s);

int main(int argc, char *argv[])
{
    std::string sourcePath;
    std::string destPath;

    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];

        if (arg == "-h")
        {
            PrintHelp();
            return 0;
        }
        else if (arg == "-s")
        {
            if (i + 1 < argc)
            {
                sourcePath = argv[++i];
            }
            else
            {
                std::cerr << "-s requires a source directory/filename argument" << std::endl;
                return 1;
            }
        }
        else if (arg == "-d")
        {
            if (i + 1 < argc)
            {
                destPath = argv[++i];
            }
            else
            {
                std::cerr << "-d requires a destination directory/filename argument" << std::endl;
                return 1;
            }
        }
    }

    // load files from source directory
    std::vector<std::string> sourceFiles;
    if (!sourcePath.empty())
    {
        if (fs::is_directory(sourcePath))
        {
            sourceFiles = TraverseDirectory(sourcePath);
        }
        else
        {
            sourceFiles.push_back(sourcePath);
        }
    }

    if (sourceFiles.empty())
    {
        PrintHelp();
        return 1;
    }

    if (destPath.empty())
    {
        std::cerr << "No destination path specified" << std::endl;
        return 1;
    }

    BuildTwiddleTable();
    ProcessFiles(sourceFiles, destPath);
}

void PrintHelp()
{
    std::cout << "Usage: " << std::endl;
    std::cout << "  -s <source>  Source directory or file" << std::endl;
    std::cout << "  -d <dest>    Destination directory or file" << std::endl;
    std::cout << "  -h           Print this help message" << std::endl;
    std::cout << "Example: -s image.pvr -d image.png" << std::endl;
    std::cout << "         -s image.png -d image.pvr" << std::endl;
    std::cout << "         -s image.pvr -d ./" << std::endl;
    std::cout << "         -s source_dir/ -d output_dir/" << std::endl;
}

std::vector<std::string> TraverseDirectory(const fs::path& directory)
{
    std::vector<std::string> files;

    try {
        for (const auto& entry : fs::directory_iterator(directory)) {
            if (entry.is_regular_file()) {
                files.push_back(entry.path());
            } else if (entry.is_directory()) {
                TraverseDirectory(entry.path());  // Recursive call for subdirectories
            }
        }
    } catch (const std::filesystem::filesystem_error& ex) {
        std::cerr << "Error accessing directory: " << ex.what() << '\n';
    }

    return files;
}

void ProcessFiles(const std::vector<std::string>& sourceFiles, const std::string& destPath)
{
    for (const auto& file : sourceFiles)
    {        
        std::string srcExt = fs::path(file).extension().string();
        std::string destExt = fs::path(destPath).extension().string();
        tolower(srcExt);
        tolower(destExt);

        std::string destFile = destPath;
        if (fs::is_directory(destPath)) {
            if (destPath[destPath.length() - 1] != '/') {
                destFile += "/";
            }
            // append filename to destination path without extension
            destFile += fs::path(file).stem().string();
            if (srcExt == ".pvr") {
                destFile += ".png";
            } else if (srcExt == ".png") {
                destFile += ".pvr";
            }
        }

        int ret = 0;
        if (srcExt == ".pvr") {
            ret = ConvertPvr2Png(file, destFile);
        } else if (srcExt == ".png") {
            ret = ConvertPng2Pvr(file, destFile);
        } else {
            std::cout << "Unknown file type, skipping: " << file << std::endl;
        }

        if (ret == 0) {
            std::cerr << "Error converting file: " << file << std::endl;
            continue;
        } else {
            std::cout << "Converted " << file << " to " << destFile << std::endl;
        }
    }
}

int ConvertPvr2Png(const std::string& sourceFile, const std::string& destFile)
{
    unsigned char* image = nullptr;
    unsigned long int imageSize = 0;
    PVRTHeader pvrtHeader;
    int ret = LoadPVRFromFile(sourceFile.c_str(), &image, &imageSize, &pvrtHeader);
    if (ret == 0)
    {
        std::cerr << "Error loading PVR file: " << sourceFile << std::endl;
        return 0;
    }

    ret = stbi_write_png(destFile.c_str(), pvrtHeader.width, pvrtHeader.height, 4, image, pvrtHeader.width * 4);
    if (ret == 0)
    {
        std::cerr << "Error saving PNG file: " << destFile << std::endl;
        return 0;
    }

    return 1;
}

int ConvertPng2Pvr(const std::string& sourceFile, const std::string& destFile)
{
    std::cout << "Not implemented yet" << std::endl;
    return 1;
}

void tolower(std::string& s)
{
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ 
        return std::tolower(c); 
    });
}