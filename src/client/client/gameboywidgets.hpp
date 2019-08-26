#ifndef H_AMBER_CLIENT_GAMEBOYWIDGETS
#define H_AMBER_CLIENT_GAMEBOYWIDGETS

#include <client/api.hpp>

#include <imgui/imgui.h>

namespace Amber::Gameboy
{
	class Registers;
}

namespace Amber::Client
{
	void DrawRegisterWindow(const char* a_Name, Gameboy::Registers& a_Registers, bool* a_Open = nullptr, ImGuiWindowFlags a_Flags = ImGuiWindowFlags_None);
	void DrawRegisterWindowContent(Gameboy::Registers& a_Registers);
}

#endif