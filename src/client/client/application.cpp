#include <client/application.hpp>

#include <client/disassembly.hpp>
#include <client/gameboywidgets.hpp>

#include <gameboy/cpu.hpp>
#include <gameboy/disassembly.hpp>
#include <gameboy/registers.hpp>

#include <common/ram.h>

#include <imgui/imgui.h>
#include <imgui/imgui_memory_editor.h>

#include <fstream>

using namespace Demu;
using namespace Client;

void Application::Tick()
{
	const auto dock_id = ImGui::DockSpaceOverViewport();

	// Load cartridge
	static auto memory = []
	{
		std::ifstream file("C:\\ROMs\\test.gb", std::ios::binary | std::ios::ate);
		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		Common::RAM<uint16_t, false> ram(size);
		file.read(reinterpret_cast<char*>(ram.GetData()), size);

		return ram;
	}();

	// Initialize cpu
	static auto cpu = []
	{
		Gameboy::CPU cpu(memory, Gameboy::GameboyType::Classic);
		cpu.Reset();
		return cpu;
	}();

	// Show debugger
	if (ImGui::Begin("Debugger"))
	{
		if (ImGui::Button("Step forward"))
		{
			cpu.ExecuteNextInstruction();
		}
	}
	ImGui::End();

	// Show registers
	ImGui::SetNextWindowDockID(dock_id, ImGuiSetCond_FirstUseEver);
	DrawRegisterWindow("Registers", cpu.GetRegisters());

	// Show disassembly
	static Gameboy::Disassembly disassembly(memory);

	static DisassemblyState disassembly_state;
	disassembly_state.m_Disassembly = &disassembly;
	disassembly_state.m_ViewAddress = cpu.GetRegisters().GetPC();

	ImGui::SetNextWindowDockID(dock_id, ImGuiSetCond_FirstUseEver);
	if (ImGui::Begin("Disassembly"))
	{
		ShowDisassembly("disassembly", disassembly_state);
	}
	ImGui::End();
}
