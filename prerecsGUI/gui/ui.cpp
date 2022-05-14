#include "ui.h"
#include "style.h"
#include "../globals.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imfilebrowser.h"
#include "../prerecs/prerecs.h"
#include "../font/IconsForkAwesome.h"
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

std::string temp;

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

void ui::createDir()
{
    globals.appdata = getAppdata();
    globals.appdata += "\\prerecsGUI";
    globals.dir = globals.appdata;
    strcpy(globals.fname, globals.dir.c_str());
}

void ui::render() 
{
    if (!globals.active) return;

    ImGui::SetNextWindowPos(ImVec2(window_pos.x, window_pos.y), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(window_size.x, window_size.y));
    ImGui::SetNextWindowBgAlpha(1.f);

    ImGuiStyle* style = &ImGui::GetStyle();

    temp = globals.fname;
    globals.appdata = temp;

    ImGui::Begin(window_title, &globals.active, window_flags);
    {
        if (globals.startOption == 3) {
            ImGui::SetNextWindowPos(ImVec2(1920/2.3, 1080/ 2.3), ImGuiCond_Once); // only center on 1920p displays atm
            ImGui::SetNextWindowSize(ImVec2(300, 150));
            ImGui::OpenPopup("Source Selection");
        }

        if (ImGui::BeginPopupModal("Source Selection", NULL, ImGuiWindowFlags_NoDecoration))
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

        ImGui::SetCursorPos({ 20,35 });
        if (ImGui::BeginChild("##Queue", ImVec2(140,380))) 
        {
            std::string que = "Queue " + std::to_string(globals.locations.size());
            ImGui::SetCursorPos({ 5.f, 5.f });
            {
                ImGui::Text(que.c_str());
                ImGui::Separator();

                for (int i = 0; i < globals.locations.size(); i++)
                {
                    if (globals.locationsDisplay[i] == "")
                        globals.locationsDisplay[i] = globals.locations[i].substr(globals.locations[i].find_last_of("/\\") + 1);

                    ImGui::Text(globals.locationsDisplay[i].c_str());

                    if (ImGui::IsItemHovered()) 
                    {
                        ImGui::SetTooltip(globals.locations.at(i).c_str());
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
                if (ImGui::Button(ICON_FK_FILTER"", { 30,30 }));
                // Insert custom ffmpeg args 
            }
            ImGui::SetCursorPos({ 92, 435 });
            {
                if (ImGui::Button(ICON_FK_FILE"", { 30,30 }));
                    // Select config (Output directory, custom ffmpeg args
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
                    std::fill(globals.locationsDisplay, globals.locationsDisplay + globals.locationsDisplay->size(), "");
                    globals.locations.clear();
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
                                        if (ImGui::Button("", { 40,40 })) globals.codec = "xvid";
                                        {
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
                                        }
                                        ImGui::PushFont(globals.arial);
                                        if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Will export a .avi mediafile"); }
                                        ImGui::PopFont();
                                    }
                                    // PRORES Button
                                    ImGui::SetCursorPos({ 120,20 });
                                    {
                                        if (ImGui::Button(" ", { 75,35 })) globals.codec = "prores";

                                        {
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
                                        }
                                        ImGui::PushFont(globals.arial);
                                        if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Will export a .mov mediafile"); }
                                        ImGui::PopFont();
                                    }

                                    // PNG Button
                                    ImGui::SetCursorPos({ 235,20 });
                                    {
                                        if (ImGui::Button("  ", { 45,35 })) globals.codec = "png";
                                        {
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
                                        }
                                        ImGui::PushFont(globals.arial);
                                        if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Will export a png sequence"); }
                                        ImGui::PopFont();
                                    }

                                    // TGA Button
                                    ImGui::SetCursorPos({ 320,20 });
                                    {
                                        if (ImGui::Button("   ", { 40,30 })) globals.codec = "tga";
                                        {
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
                        globals.convdir = temp + "\\" + getTime().str();

                        _mkdir(globals.convdir.c_str());

                        std::cout << "start";

                        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)startEncode, 0, 0, 0);
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
                ImGui::TextWrapped(globals.cnsl.c_str());
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
                globals.locations.push_back(loc);
            }

            globals.fileDialog.ClearSelected();

        }

        if (globals.folderDialog.HasSelected())
        {
            std::string path = globals.folderDialog.GetSelected().u8string();

            for (const auto& file : std::filesystem::directory_iterator(path))
            {
                globals.locations.push_back(file.path().u8string());
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