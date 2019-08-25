#include <client/application.hpp>

#include <client/disassembly.hpp>
#include <client/gameboywidgets.hpp>

#include <gameboy/cpu.hpp>
#include <gameboy/debugger.hpp>
#include <gameboy/registers.hpp>

#include <common/ram.h>

#include <imgui/imgui.h>

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

		Common::RAM<uint16_t, false> ram(0x10000);
		file.read(reinterpret_cast<char*>(ram.GetData()), std::min<size_t>(size, ram.GetSize()));

		return ram;
	}();

	// Initialize cpu
	static auto cpu = []
	{
		Gameboy::CPU cpu(memory, Gameboy::GameboyType::Classic);
		cpu.Reset();
		return cpu;
	}();

	ImGui::ShowDemoWindow();

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
	static Gameboy::Debugger debugger(cpu);

	static DisassemblyState disassembly_state;
	disassembly_state.m_Debugger = &debugger;
	disassembly_state.m_ViewAddress = cpu.GetRegisters().GetPC();

	ImGui::SetNextWindowDockID(dock_id, ImGuiSetCond_FirstUseEver);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	if (ImGui::Begin("Disassembly"))
	{
		ShowDisassembly("disassembly", disassembly_state);
	}
	ImGui::PopStyleVar();
	ImGui::End();
}
