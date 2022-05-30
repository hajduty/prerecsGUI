#include "ui.h"
#include "style.h"
#include "../globals.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imfilebrowser.h"
#include "../prerecs/prerecs.h"
#include "../font/IconsForkAwesome.h"
#include "../config/config.h"
#include <fstream>
#include <chrono>
#include <filesystem>
#include <direct.h>
#include <iostream>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <ctime>
#include <vector>
#include <cstring>
#include <cctype>

void textCheckbox(std::string text, ImVec2 size, bool* p, ImVec2 pos)
{
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
    ImGui::SetCursorPos({ pos.x, pos.y });

    ImGui::PushFont(globals.arialMedium);
    std::string bName = "##" + text;
    if (ImGui::Button(bName.c_str(), { size.x,size.y }))
    {
        *p = !*p;
        std::cout << *p;
    }

    ImGui::SetCursorPos({ pos.x, pos.y });
    {
        if (*p == false)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 0.15));
            ImGui::Text(text.c_str());
            ImGui::PopStyleColor();
        }
        else {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.25, 0.25, 1));
            ImGui::Text(text.c_str());
            ImGui::PopStyleColor();
        }
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(2);
}

// generate folder name 
auto getTime() {
    std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);
    std::stringstream da;
    da << now->tm_yday << "_" << globals.codec;
    std::cout << da.str();
    return da;
}

std::string getAppdata() 
{
    char* buf = nullptr;
    size_t sz = 0;
    if (_dupenv_s(&buf, &sz, "APPDATA") == 0 && buf != nullptr)
    {
        std::string t(buf);
        free(buf);
        return t;
    }
}

// Get appdata folder, & set variables
void ui::createDir()
{
    globals.appdata = getAppdata();
    globals.appdata += "\\prerecsGUI";
    config::appdata = globals.appdata;
    globals.dir = globals.appdata;
    strcpy(globals.fname, globals.dir.c_str());
}

int temptemptemp = 1;
void ui::render() 
{
    if (!globals.active) 
        return;

    ImGui::SetNextWindowPos(ImVec2(window_pos.x, window_pos.y), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(window_size.x, window_size.y));
    ImGui::SetNextWindowBgAlpha(1.f);

    ImGuiStyle* style = &ImGui::GetStyle();
    ImGuiIO& io = ImGui::GetIO();

    // no clue why i did this, but it works
    ui::temp = globals.fname;
    globals.appdata = ui::temp;

    ImGui::Begin(window_title, &globals.active, window_flags);
    {
        if (globals.startOption == 3) {
            if (temptemptemp) {
                ImGui::SetNextWindowPos(ImVec2(1920 / 2.3, 1080 / 2.3));
                temptemptemp = 0;
            }

            ImGui::OpenPopup("Source Selection");
        }

        if (ui::argsOpen) {
            ImGui::OpenPopup("FFmpeg Args");
        }

        if (ui::configOpen) {
            ImGui::OpenPopup("Configs");
        }

        // source selection modal
        ImGui::SetNextWindowSize(ImVec2(300, 150));
        if (ImGui::BeginPopupModal("Source Selection", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove))
        {

            static ImVec4 fileText = ImVec4(0.15, 0.15, 0.15, 1);
            static ImVec4 folderText = ImVec4(0.15, 0.15, 0.15, 1);

            ImGui::SetCursorPos({ 100,5 });
            ImGui::Text("Source Selection");
            ImGui::Separator();
            ImGui::PushFont(globals.bigIcon);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 0.5));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
            {
                ImGui::SetCursorPos({ 0,45 });
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, folderText);

                    if (ImGui::Button(ICON_FK_FOLDER_OPEN"", { 150,65 })) {
                        globals.startOption = 2;
                        ImGui::CloseCurrentPopup();
                    }
                    
                    if (ImGui::IsItemHovered()) {
                        folderText = ImVec4(1, 0.25, 0.25, 1);
                        ImGui::PushFont(globals.arial);
                        ImGui::SetTooltip("Select folders containing media files");
                        ImGui::PopFont();
                    }
                    else {
                        folderText = ImVec4(0.25, 0.25, 0.25, 1);
                    }
                    ImGui::PopStyleColor();
                }
                ImGui::SetCursorPos({ 150,45 });
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, fileText);

                    if (ImGui::Button(ICON_FK_FILE"", { 150,65 })) {
                        globals.startOption = 1;
                        ImGui::CloseCurrentPopup();
                    }

                    if (ImGui::IsItemHovered()) {
                        ImGui::PushFont(globals.arial);
                        ImGui::SetTooltip("Select individual media files");
                        ImGui::PopFont();
                        fileText = ImVec4(1, 0.25, 0.25, 1);
                    }
                    else {
                        fileText = ImVec4(0.25, 0.25, 0.25, 1);
                    }
                    ImGui::PopStyleColor();
                }
            }
            ImGui::PopStyleColor(3);

            ImGui::PopFont();
            ImGui::Spacing();
      
            ImGui::EndPopup();
        }

        // ffmpeg args modal
        ImGui::SetNextWindowSize(ImVec2(450, 90));
        if (ImGui::BeginPopupModal("FFmpeg Args", &ui::argsOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
        {
            ImGui::SetCursorPos({ 6,40 });
            {
                ImGui::PushItemWidth(450);

                if (globals.argsDisplay[0] == '\0');
                strcpy(globals.argsDisplay, globals.args.c_str());

                ImGui::InputText("####", globals.argsDisplay, IM_ARRAYSIZE(globals.argsDisplay));
                globals.args = globals.argsDisplay;
                globals.args;
                ImGui::PopItemWidth();
            }
            ImGui::EndPopup();
        }

        // configs modal
        ImGui::SetNextWindowSize(ImVec2(400, 300));
        if (ImGui::BeginPopupModal("Configs", &ui::configOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove))
        {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0.4));
            {
                ImGui::SetCursorPos({ 0,20 });
                if (ImGui::BeginChild("##Checkboxes", ImVec2(200, 140)))
                {
                    textCheckbox("SAVE FFMPEG ARGS", { 200,40 }, &config::saveArgs, ImVec2(25, 15));

                    textCheckbox("SAVE OUTPUT DIR", { 200,40 }, &config::saveDir, ImVec2(32, 55));

                    textCheckbox("SAVE QUEUE", { 200,40 }, &config::saveQueue, ImVec2(52, 95));
                    ImGui::EndChild();
                }
                ImGui::PopStyleColor();
            }

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.25f));
            {
                ImGui::SetCursorPos({ 0,270 });
                {
                    if (ImGui::Button("LOAD", { 100,30 }))
                    {
                        config::loadConfig();
                        ui::configOpen = !ui::configOpen;
                    }
                }
                ImGui::SetCursorPos({ 100,270 });
                {
                    if (ImGui::Button("SAVE", { 100,30 }))
                    {
                        config::saveConfig();
                        ui::configOpen = !ui::configOpen;
                    }
                }
                ImGui::PopStyleColor();
            }

            ImGui::SetCursorPos({ 0,247 });
            {
                ImGui::PushFont(globals.arial);
                ImGui::PushItemWidth(200);
                ImGui::InputText("##", config::configName, IM_ARRAYSIZE(config::configName));
                ImGui::PopItemWidth();
                ImGui::PopFont();
            }

            std::string filePath;
            std::string lastof;
            std::string ee;
            bool clicked = false;
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.10f));
            {
                ImGui::SetCursorPos({ 200,0 });
                {
                    if (ImGui::BeginChild("##Config", { 200,300 }))
                    {
                        ImGui::SetCursorPos({ 0,30 });
                        std::string path = config::appdata + "\\configs\\";

                        for (const auto& file : std::filesystem::directory_iterator(path))
                        {
                            filePath = file.path().u8string();
                            lastof = filePath.substr(filePath.find_last_of("/\\") + 1);

                            if (lastof == config::configName) {
                                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.25, 0.25, 1));
                                ImGui::Text(lastof.c_str());
                                ImGui::PopStyleColor();
                            }
                            else {
                                ImGui::Text(lastof.c_str());
                            }

                            if (ImGui::IsItemHovered())
                            {
                                if (io.MouseDown[0]) {

                                    std::cout << "clicked" << "\n";
                                    ee = filePath;
                                    clicked = !clicked;
                                    strcpy(config::configName, lastof.c_str());
                                }
                            }
                        }
                        ImGui::EndChild();
                    }
                }
                ImGui::PopStyleColor();

                ImGui::SetCursorPos({ 0,140 });
                {
                    if (ImGui::BeginChild("##ConfigData", { 200,110 }, window_flags))
                    {
                        static std::string str;

                        if (clicked) {
                            std::ifstream t(ee);
                            std::stringstream buffer;
                            buffer << t.rdbuf();
                            str = buffer.str();
                            t.close();
                            clicked = !clicked;
                        }

                        ImGui::PushFont(globals.arialSmall);
                        {
                            if (str != "")
                                ImGui::TextWrapped(str.c_str());

                            ImGui::PopFont();
                        }
                        ImGui::EndChild();
                    }
                }
            }
            ImGui::EndPopup();
        }

        // child queue
        ImGui::SetCursorPos({ 20,35 });
        if (ImGui::BeginChild("##Queue", ImVec2(140,380))) 
        {
            std::string que = "Queue " + std::to_string(globals.locations.size());
            ImGui::SetCursorPos({ 5.f, 5.f });
            {
                ImGui::Text(que.c_str());
                ImGui::Separator();

                ImVec4 error = ImVec4(1, 0.5, 0.5, 1);
                ImVec4 finished = ImVec4(0.5, 1, 0.5, 1);
                ImVec4 started = ImVec4(1, 1, 0.5, 1);

                for (int i = 0; i < globals.locations.size(); i++)
                {
                   if (globals.locationsDisplay.at(i) == "")
                       globals.locationsDisplay.at(i) = globals.locations[i].substr(globals.locations[i].find_last_of("/\\") + 1);
                                       
                   if (globals.locationsDisplay.at(i).find("Finished") != std::string::npos)
                   {
                       ImGui::PushStyleColor(ImGuiCol_Text, finished);
                   }
                   else if (globals.locationsDisplay.at(i).find("Error") != std::string::npos)
                   {
                       ImGui::PushStyleColor(ImGuiCol_Text, error);
                   }
                   
                   else if (globals.locationsDisplay.at(i).find("Started") != std::string::npos)
                   {
                       ImGui::PushStyleColor(ImGuiCol_Text, started);
                   }
                   else
                   {
                       ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
                   }
                   
                   
                   ImGui::Text(globals.locationsDisplay[i].c_str());
                   
                   ImGui::PopStyleColor();
                   
                   if (ImGui::IsItemHovered()) 
                   {
                       static std::string errorMsg;
                       static std::string errorLoc;

                       ImGui::BeginTooltip();
                       {
                           ImGui::Text(globals.locations.at(i).c_str());
                           ImGui::Separator();
                           ImGui::Text(globals.locationsDisplay[i].c_str());

                           if (globals.locationsDisplay.at(i).find("Error") != std::string::npos)
                           {
                               ImGui::Separator();
                               ImGui::TextColored(ImVec4(1,0.25,0.25,1), "Hold down for info");

                               if (io.MouseDown[0])
                               {
                                   errorLoc = globals.locationsDisplay.at(i);
                                   errorMsg = globals.cnsl.at(i);
                               }

                               if (errorLoc == globals.locationsDisplay.at(i))
                               {
                                   ImGui::Separator();
                                   ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 0.25, 1));
                                   ImGui::TextUnformatted(errorMsg.c_str());
                                   ImGui::PopStyleColor();

                                   if (io.MouseDown[0])
                                   {
                                       errorLoc = "";
                                   }
                               }
                           }
                           ImGui::EndTooltip();
                       }
                   }
                }
                
            }
            ImGui::EndChild();
        }
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.11f, 0.11f, 0.11f, 1.00f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.25, 0.25, 1));
        ImGui::PushFont(globals.arialBig);
        {
            ImGui::SetCursorPos({ 22,435 });
            {
                if (ImGui::Button(ICON_FK_COGS"", { 30,30 }))
                    globals.startOption = 3;
            }
            ImGui::SetCursorPos({ 57, 435 });
            {
                if (ImGui::Button(ICON_FK_FILTER"", { 30,30 })) 
                {
                    ui::argsOpen = !ui::argsOpen;
                }
                // Insert custom ffmpeg args 
            }
            ImGui::SetCursorPos({ 92, 435 });
            {
                if (ImGui::Button(ICON_FK_FILE"", { 30,30 }))
                    ui::configOpen = !ui::configOpen;

                    // Select config (Output directory, custom ffmpeg args)
            }
            ImGui::SetCursorPos({ 127, 435 });
            {
                if (ImGui::Button(ICON_FK_INFO"", { 30,30 }));
                // Info
            }
        }
        ImGui::PopFont();
        ImGui::PopStyleColor(2);

        ImGui::SetCursorPos({ 180,35 });
        if (ImGui::BeginChild("##Controls", ImVec2(400, 240))) 
        {
            ImGui::SetCursorPos({ 5.f,5.f }); 
            {
                ImGui::Text("Controls");
                ImGui::Separator();
            }

            ImGui::SetCursorPos({ 0,25 });
            {
                if (ImGui::Button("LOAD FILES", { 150,74 })) {
                    if (globals.startOption == 1)
                        globals.fileDialog.Open();
                    else
                        globals.folderDialog.Open();
                }
                if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Load your media files"); }
            }
            ImGui::SetCursorPos({ 150,25 }); 
            {
                if (ImGui::Button("OPEN DIR", { 100,74 }))
                {
                    _mkdir(globals.appdata.c_str());
                    ShellExecuteA(NULL, "open", globals.appdata.c_str(), NULL, NULL, SW_SHOWDEFAULT);
                }
                if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Open output directory"); }
            }

            ImGui::SetCursorPos({ 250,25 }); 
            {
                if (ImGui::Button("CLEAR QUEUE", {150,74}))
                {
                    globals.locationsDisplay.clear();
                    globals.locations.clear();
                    globals.cnsl.clear();
                }
                if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Clear media queue to select new files"); }
            }
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.f);
            {
                ImGui::SetCursorPos({ 0,99 });
                {
                    ImGui::PushItemWidth(400);
                    ImGui::InputText("###", globals.fname, IM_ARRAYSIZE(globals.fname));
                    ImGui::PopItemWidth();
                    if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Output directory (If empty, will output converted files in a new folder in source directory)"); }
                }
                ImGui::SetCursorPos({ 0,120 });
                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4({ 0.0f, 0.0f, 0.0f, 0.25f }));
                {
                    if (ImGui::BeginChild("##Codecs", ImVec2(400, 80))) 
                    {
                        ImGui::PushFont(globals.arialBig);
                        {
                            {
                                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 0.5));
                                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
                                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
                                {
                                    // XVID Button
                                    ImGui::SetCursorPos({ 40,20 });
                                    {
                                        if (ImGui::Button("", { 40,40 }))
                                        {
                                            globals.codec = "xvid";
                                            globals.args = "\" -c:v mpeg4 -vtag xvid -qscale:v 1 -qscale:a 1 -g 32 -vsync 1 -y \"";
                                            globals.filetype = ".avi";
                                        }
                                        
                                        ImGui::SetCursorPos({ 40, 25 });
                                        {
                                            if (globals.codec != "xvid")
                                            {
                                                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 0.20));
                                                ImGui::Text("XVID");
                                                ImGui::PopStyleColor();
                                            }
                                            else {
                                                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.25, 0.25, 1));
                                                ImGui::Text("XVID");
                                                ImGui::PopStyleColor();
                                            }
                                        }
                                        
                                        ImGui::PushFont(globals.arial);
                                        if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Will export a .avi mediafile"); }
                                        ImGui::PopFont();
                                    }
                                    // PRORES Button
                                    ImGui::SetCursorPos({ 120,20 });
                                    {
                                        if (ImGui::Button(" ", { 75,35 })) 
                                        {
                                            globals.codec = "prores";
                                            globals.args = "\" -c:v prores_ks -profile:v 3 -c:a pcm_s16le -y \"";
                                            globals.filetype = ".mov";
                                        }

                                        ImGui::SetCursorPos({ 120, 25 });
                                        {
                                            if (globals.codec != "prores")
                                            {
                                                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 0.20));
                                                ImGui::Text("PRORES");
                                                ImGui::PopStyleColor();
                                            }
                                            else {
                                                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.25, 0.25, 1));
                                                ImGui::Text("PRORES");
                                                ImGui::PopStyleColor();
                                            }
                                        }

                                        ImGui::PushFont(globals.arial);
                                        if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Will export a .mov mediafile"); }
                                        ImGui::PopFont();
                                    }

                                    // PNG Button
                                    ImGui::SetCursorPos({ 235,20 });
                                    {
                                        if (ImGui::Button("  ", { 45,35 }))
                                        {
                                            globals.codec = "png";
                                            globals.args = "\" -c:v mpeg4 -vtag xvid -qscale:v 1 -qscale:a 1 -g 32 -vsync 1 -y \"";
                                            globals.filetype = "-%04d.png";
                                        }

                                        ImGui::SetCursorPos({ 235, 25 });
                                        {
                                            if (globals.codec != "png")
                                            {
                                                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 0.20));
                                                ImGui::Text("PNG");
                                                ImGui::PopStyleColor();
                                            }
                                            else {
                                                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.25, 0.25, 1));
                                                ImGui::Text("PNG");
                                                ImGui::PopStyleColor();
                                            }
                                        }
                                        
                                        ImGui::PushFont(globals.arial);
                                        if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Will export a png sequence"); }
                                        ImGui::PopFont();
                                    }

                                    // TGA Button
                                    ImGui::SetCursorPos({ 320,20 });
                                    {
                                        if (ImGui::Button("   ", { 40,30 }))
                                        {
                                            globals.codec = "tga";
                                            globals.args = "\" -y \"";
                                            globals.filetype = "-%04d.tga";
                                        }

                                        ImGui::SetCursorPos({ 320, 25 });
                                        {
                                            if (globals.codec != "tga")
                                            {
                                                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 0.20));
                                                ImGui::Text("TGA");
                                                ImGui::PopStyleColor();
                                            }
                                            else {
                                                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.25, 0.25, 1));
                                                ImGui::Text("TGA");
                                                ImGui::PopStyleColor();
                                            }
                                        }
                                        
                                        ImGui::PushFont(globals.arial);
                                        if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Will export a tga sequence"); }
                                        ImGui::PopFont();
                                    }
                                }
                                ImGui::PopStyleColor(3);
                            }
                            ImGui::PopFont();
                        }
                        ImGui::EndChild();
                    }
                    ImGui::PopStyleColor();
                }
                ImGui::PopStyleVar();
            }
            
            ImGui::SetCursorPos({ 0.f, 200.f });
            {
                if (ImGui::Button(globals.startBtn.c_str(), { 400,40.f })) 
                {
                    if (!globals.start) 
                    {
                        globals.startBtn = "START";
                        std::cout << "stop";
                        globals.stopEncode = true;
                        Sleep(50);
                    }

                    if (globals.start && !globals.stopEncode) 
                    {
                        globals.startBtn = "STOP";
                        globals.convdir = ui::temp + "\\" + getTime().str();

                        _mkdir(globals.convdir.c_str());

                        std::cout << "start";

                        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)prerecs::startEncode, 0, 0, 0);
                    }
                    globals.start = !globals.start;
                }
                if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Start converting queue"); }
            }

            ImGui::EndChild();
        }

        ImGui::SetCursorPos({ 180,295 });
        {
            if (ImGui::BeginChild("##Output", ImVec2(400, 170), false, window_flags))
            {
                ImGui::SetCursorPos({ 5.f,5.f });
                ImGui::TextWrapped("Console");
                ImGui::Separator();
                for (int i = 0; i < globals.cnsl.size(); i++)
                {
                    ImGui::TextWrapped(globals.cnsl.at(i).c_str());
                }
                ImGui::EndChild();
            }
        }

        if (globals.fileDialog.HasSelected())
        {
            //std::cout << "Loaded filename" << std::endl;
            globals.fileDialog.GetMultiSelected();

            std::vector selectedFiles = globals.fileDialog.GetMultiSelected();

            for (int i = 0; i < selectedFiles.size(); i++)
            {
                std::string loc = selectedFiles.at(i).u8string();
                std::string lastof = loc.substr(loc.find_last_of("/\\") + 1);

                globals.locations.push_back(loc);
                globals.locationsDisplay.push_back(lastof);

            }

            globals.fileDialog.ClearSelected();

        }

        if (globals.folderDialog.HasSelected())
        {
            std::string path = globals.folderDialog.GetSelected().u8string();

            for (const auto& file : std::filesystem::directory_iterator(path))
            {
                std::string f = file.path().u8string();
                std::string lastof = f.substr(f.find_last_of("/\\") + 1);
                std::array<std::string, 5> a{ ".png", ".tga", ".avi", ".mp4", ".mov" };

                auto it = std::find_if(begin(a), end(a), [&](const std::string& s) { return f.find(s) != std::string::npos; });

                if (it != end(a))
                {
                    globals.locations.push_back(f);
                    globals.locationsDisplay.push_back(lastof);
                }

            }

            globals.folderDialog.ClearSelected();
        }

        globals.fileDialog.Display();

        globals.folderDialog.Display();
        //ImGui::PopFont();
    }
    ImGui::End();
}

void ui::init(LPDIRECT3DDEVICE9 device) 
{
    dev = device;
	
    // colors
    ModernDarkTheme();

	if (window_pos.x == 0) {
		RECT screen_rect{};
		GetWindowRect(GetDesktopWindow(), &screen_rect);
		screen_res = ImVec2(float(screen_rect.right), float(screen_rect.bottom));
		window_pos = (screen_res - window_size) * 0.5f;
	}
}