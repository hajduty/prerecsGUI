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
    std::string args;
    std::string filetype;

    if (globals.codec == "xvid") 
    {
        args = "\" -c:v mpeg4 -vtag xvid -qscale:v 1 -qscale:a 1 -g 32 -vsync 1 -y \"";
        filetype = ".avi";
    }
    else if (globals.codec == "prores")
    {
        args = "\" -c:v prores_ks -profile:v 3 -c:a pcm_s16le -y \"";
        filetype = ".mov";
    }
    else if (globals.codec == "png")
    {
        args = "\" -y \"";
        filetype = "-%04d.png";
    }
    else if (globals.codec == "tga")
    {
        args = "\" -y \"";
        filetype = "-%04d.tga";
    }
    else 
    {
        exit(1);
    }

    for (int i = 0; i < globals.locations.size(); i++)
    {
        std::string base_filename = globals.locations[i].substr(globals.locations[i].find_last_of("/\\") + 1);
        std::string temp = globals.file[i];
        std::filesystem::path p = base_filename;

        if (globals.stopEncode) 
        {
            globals.stopEncode = false;
            break;
        }

        cmd = "ffmpeg -i \"" + globals.locations[i] + args + globals.convdir + "\\" + globals.codec + "_" + p.replace_extension().u8string() + filetype + "\" 2>&1";
        globals.file[i] = base_filename + " - Started";
        globals.cnsl += exec(cmd.c_str());
        globals.file[i] = base_filename + " Finished";
    }
    globals.startBtn = "Start";
    globals.start = true;
}