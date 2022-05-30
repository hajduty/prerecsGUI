#include <iostream>
#include "../globals.h"
#include "prerecs.h"
#include <string>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <filesystem>
#include <utility>
#include <direct.h>

std::string prerecs::exec(const char* cmd)
{
    char moduleFilePath[MAX_PATH];
    GetModuleFileName(NULL, moduleFilePath, MAX_PATH);

    // Find the backslash in front of the name of your program.
    std::string::size_type pos = std::string(moduleFilePath).rfind("\\");

    // Replace the program name by your sub-folder.
    prerecs::subFolderPath = std::string(moduleFilePath).substr(0, pos);

    // Change into the sub-folder relative to your program.
    SetCurrentDirectory(prerecs::subFolderPath.c_str());

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
        prerecs::success = true;
        std::cout << "SUCCESS\n";
    }
    else
    {
        prerecs::success = false;
        std::cout << "FAILED\n";
    }

    return result;
}

void prerecs::startEncode()
{
    // Set directory to use ffmpeg command in
    SetCurrentDirectory(prerecs::subFolderPath.c_str());

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

        // Check if file has already been used before
        if (globals.locationsDisplay[i].find("Finished") != std::string::npos)
            continue;

        if (globals.codec == "png" | globals.codec == "tga")
        {
            if (!globals.fname[0] == '\0') // If output path is empty, output in a new folder in source directory
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

        globals.locationsDisplay.at(i) = base_filename + " - Started"; // Change displayname & status
        
        globals.cnsl.push_back(exec(cmd.c_str())); // Execute the ffmpeg command

        if (prerecs::success)
        {
            globals.locationsDisplay.at(i) = base_filename + " - Finished"; // Change displayname & status
        }
        else
        {
            globals.locationsDisplay.at(i) = base_filename + " - Error"; // Change displayname & status
        }
    }
    // Reset start button
    globals.startBtn = "Start";
    globals.start = true;
}