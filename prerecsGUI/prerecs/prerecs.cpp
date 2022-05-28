#include <iostream>
#include "../globals.h"
#include <string>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <filesystem>
#include <utility>
#include <direct.h>

std::string subFolderPath;

std::string exec(const char* cmd)
{
    char moduleFilePath[MAX_PATH];
    GetModuleFileName(NULL, moduleFilePath, MAX_PATH);

    // Find the backslash in front of the name of your program.
    std::string::size_type pos = std::string(moduleFilePath).rfind("\\");

    // Replace the program name by your sub-folder.
    subFolderPath = std::string(moduleFilePath).substr(0, pos);

    // Change into the sub-folder relative to your program.
    SetCurrentDirectory(subFolderPath.c_str());

    std::array<char, 128> buffer;
    std::string result;

    auto pipe = _popen(cmd, "r");

    if (!pipe) throw std::runtime_error("popen() failed!");

    while (!feof(pipe))
    {
        if (fgets(buffer.data(), 128, pipe) != nullptr)
            result += buffer.data();
    }

    auto rc = _pclose(pipe);

    if (rc == EXIT_SUCCESS)
    {
        std::cout << "SUCCESS\n";
    }
    else
    {
        std::cout << "FAILED\n";
    }

    return result;
}

void startEncode()
{
    SetCurrentDirectory(subFolderPath.c_str());

    std::string cmd;

    for (int i = 0; i < globals.locations.size(); i++)
    {
        std::string base_filename = globals.locations[i].substr(globals.locations[i].find_last_of("/\\") + 1);
        std::string temp = globals.locationsDisplay.at(i);
        std::filesystem::path p = base_filename;

        if (globals.stopEncode) 
        {
            globals.stopEncode = false;
            break;
        }

        if (globals.locationsDisplay[i].find("Finished") != std::string::npos)
            continue;

        std::array<std::string, 5> a{ ".png", ".tga", ".avi", ".mp4", ".mov" };

        auto it = std::find_if(begin(a), end(a),[&](const std::string& s) { return globals.locationsDisplay[i].find(s) != std::string::npos; });

        if (!(it != end(a)))
        {
            globals.locationsDisplay[i] += " - Filetype Error";
            continue;
        }

        //if (!(globals.locationsDisplay[i].find(".tga") != std::string::npos))

        if (globals.codec == "png" | globals.codec == "tga")
        {
            if (!globals.fname[0] == '\0')
            {
                std::string pngPath = globals.convdir + "\\" + p.replace_extension().u8string();
                _mkdir(pngPath.c_str());
                cmd = "ffmpeg -i \"" + globals.locations[i] + globals.args + pngPath + "\\" + globals.codec + "_" + p.replace_extension().u8string() + globals.filetype + "\" 2>&1";
            }
            else 
            {
                std::filesystem::path sp = globals.locations[i];
                std::string sourcePath = sp.replace_extension().u8string();
                std::cout << "\n" << sourcePath << "\n";
                _mkdir(sourcePath.c_str());
                cmd = "ffmpeg -i \"" + globals.locations[i] + globals.args + sourcePath + "\\" + globals.codec + "_" + p.replace_extension().u8string() + globals.filetype + "\" 2>&1";
            }
        }
        else 
        {
            if (!globals.fname[0] == '\0')
            {
                cmd = "ffmpeg -i \"" + globals.locations[i] + globals.args + globals.convdir + "\\" + globals.codec + "_" + p.replace_extension().u8string() + globals.filetype + "\" 2>&1";
            }
            else
            {
                std::filesystem::path sp = globals.locations[i];
                std::string sourcePath = sp.replace_extension().u8string();
                _mkdir(sourcePath.c_str());
                cmd = "ffmpeg -i \"" + globals.locations[i] + globals.args + sourcePath + "\\" + globals.codec + "_" + p.replace_extension().u8string() + globals.filetype + "\" 2>&1";
            }
        }
        globals.locationsDisplay.at(i) = base_filename + " - Started";
        globals.cnsl += exec(cmd.c_str());
        globals.locationsDisplay.at(i) = base_filename + " - Finished";
    }
    globals.startBtn = "Start";
    globals.start = true;
}