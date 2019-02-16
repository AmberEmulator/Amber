#ifndef H_DEMU_CLIENT_GAMEBOYWIDGETS
#define H_DEMU_CLIENT_GAMEBOYWIDGETS

#include <client/api.hpp>

#include <imgui/imgui.h>

namespace Demu::Gameboy
{
	class Registers;
}

namespace Demu::Client
{
	void DrawRegisterWindow(const char* a_Name, Gameboy::Registers& a_Registers, bool* a_Open = nullptr, ImGuiWindowFlags a_Flags = ImGuiWindowFlags_None);
	void DrawRegisterWindowContent(Gameboy::Registers& a_Registers);
}

#endif