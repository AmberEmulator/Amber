#ifndef H_AMBER_CLIENT_GAMEBOYWIDGETS
#define H_AMBER_CLIENT_GAMEBOYWIDGETS

#include <client/api.hpp>

#include <imgui/imgui.h>

namespace Amber::Gameboy
{
	class CPU;
}

namespace Amber::Client
{
	void DrawRegisterWindow(const char* a_Name, Gameboy::CPU& a_CPU, bool* a_Open = nullptr, ImGuiWindowFlags a_Flags = ImGuiWindowFlags_None);
	void DrawRegisterWindowContent(Gameboy::CPU& a_CPU);
}

#endif