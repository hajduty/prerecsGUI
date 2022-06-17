#include "../imgui/imgui.h"

void ModernDarkTheme()
{
	auto& style = ImGui::GetStyle();
	style.ChildRounding = 0;
	style.GrabRounding = 0;
	style.FrameRounding = 2;
	style.PopupRounding = 0;
	style.ScrollbarRounding = 0;
	style.TabRounding = 2;
	style.WindowRounding = 0;
	style.FramePadding = { 4, 4 };
	style.WindowPadding = { 8,8 };
	style.ScrollbarSize = 12;

	style.WindowTitleAlign = { 0.0, 0.5 };
	style.ColorButtonPosition = ImGuiDir_Left;

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = { 1.0f, 1.0f, 1.0f, 1.00f };				//
	colors[ImGuiCol_TextDisabled] = { 0.25f, 0.25f, 0.25f, 1.00f };		//
	colors[ImGuiCol_WindowBg] = { 0.085f, 0.085f, 0.085f, 1.f };		//
	colors[ImGuiCol_ChildBg] = { 0.11f, 0.11f, 0.11f, 1.00f };			//
	colors[ImGuiCol_PopupBg] = { 0.11f, 0.11f, 0.11f, 1.f };			//
	colors[ImGuiCol_Border] = { 0.07f, 0.08f, 0.08f, 1.00f };
	colors[ImGuiCol_BorderShadow] = { 0.00f, 0.00f, 0.00f, 0.00f };
	colors[ImGuiCol_FrameBg] = { 0.35f, 0.35f, 0.35f, 0.54f };			//
	colors[ImGuiCol_FrameBgHovered] = { 0.31f, 0.29f, 0.27f, 1.00f };
	colors[ImGuiCol_FrameBgActive] = { 0.40f, 0.36f, 0.33f, 0.67f };
	colors[ImGuiCol_TitleBg] = { 0.13f, 0.13f, 0.13f, 1.00f };
	colors[ImGuiCol_TitleBgActive] = { 0.13f, 0.13f, 0.13f, 1.00f };	//
	colors[ImGuiCol_TitleBgCollapsed] = { 0.0f, 0.0f, 0.0f, 0.61f };
	colors[ImGuiCol_MenuBarBg] = { 0.18f, 0.18f, 0.18f, 0.94f };		//
	colors[ImGuiCol_ScrollbarBg] = { 0.00f, 0.00f, 0.00f, 0.16f };
	colors[ImGuiCol_ScrollbarGrab] = { 0.24f, 0.22f, 0.21f, 1.00f };
	colors[ImGuiCol_ScrollbarGrabHovered] = { 0.31f, 0.29f, 0.27f, 1.00f };
	colors[ImGuiCol_ScrollbarGrabActive] = { 0.40f, 0.36f, 0.33f, 1.00f };
	colors[ImGuiCol_CheckMark] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
	colors[ImGuiCol_SliderGrab] = { 0.8f, 0.25f, 0.25f, 1.0f };			//		
	colors[ImGuiCol_SliderGrabActive] = { 1.f, 0.25f, 0.25f, 1.00f }; //
	colors[ImGuiCol_Button] = { 0.55f, 0.55f, 0.55f, 0.00f };			//
	colors[ImGuiCol_ButtonHovered] = { 0.15f, 0.15f, 0.15f, 1.f };	//	
	colors[ImGuiCol_ButtonActive] = { 0.60f, 0.60f, 0.60f, 1.00f };		//
	colors[ImGuiCol_Header] = ImVec4(0.98f, 0.26f, 0.26f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.98f, 0.26f, 0.26f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.75f, 0.10f, 0.10f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.75f, 0.10f, 0.10f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = { 1.0f, 1.0f, 1.0f, 0.25f };			//
	colors[ImGuiCol_ResizeGripHovered] = { 1.00f, 1.0f, 1.0f, 0.4f };	//
	colors[ImGuiCol_ResizeGripActive] = { 1.00f, 1.00f, 1.0f, 0.95f };	//
	colors[ImGuiCol_Tab] = { 0.18f, 0.18f, 0.18f, 1.0f };				//
	colors[ImGuiCol_TabHovered] = { 0.58f, 0.58f, 0.58f, 0.80f };		//
	colors[ImGuiCol_TabActive] = { 0.3f, 0.30f, 0.30f, 1.00f };
	colors[ImGuiCol_TabUnfocused] = { 0.07f, 0.10f, 0.15f, 0.97f };
	colors[ImGuiCol_TabUnfocusedActive] = { 0.14f, 0.26f, 0.42f, 1.00f };
	colors[ImGuiCol_PlotLines] = { 0.66f, 0.60f, 0.52f, 1.00f };
	colors[ImGuiCol_PlotLinesHovered] = { 0.98f, 0.29f, 0.20f, 1.00f };
	colors[ImGuiCol_PlotHistogram] = { 0.60f, 0.59f, 0.10f, 1.00f };
	colors[ImGuiCol_PlotHistogramHovered] = { 0.72f, 0.73f, 0.15f, 1.00f };
	colors[ImGuiCol_TextSelectedBg] = { 0.27f, 0.52f, 0.53f, 0.35f };
	colors[ImGuiCol_DragDropTarget] = { 0.60f, 0.59f, 0.10f, 0.90f };
	colors[ImGuiCol_NavHighlight] = { 0.51f, 0.65f, 0.60f, 1.00f };
	colors[ImGuiCol_NavWindowingHighlight] = { 1.00f, 1.00f, 1.00f, 0.70f };
	colors[ImGuiCol_NavWindowingDimBg] = { 0.80f, 0.80f, 0.80f, 0.00f };
	colors[ImGuiCol_ModalWindowDimBg] = { 0, 0, 0, 0.60f };
}
