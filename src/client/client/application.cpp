#include <client/application.hpp>

#include <client/disassembly.hpp>
#include <client/gameboywidgets.hpp>

#include <gameboy/cpu.hpp>
#include <gameboy/debugger.hpp>
#include <gameboy/registers.hpp>

#include <common/ram.hpp>
#include <common/mmu.hpp>

#include <imgui/imgui.h>

#include <fstream>

using namespace Amber;
using namespace Client;

void Application::Tick()
{
	const auto dock_id = ImGui::DockSpaceOverViewport();

	// Load cartridge
	static auto cartridge = []
	{
		std::ifstream file("C:\\ROMs\\test.gb", std::ios::binary | std::ios::ate);
		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		Common::RAM<uint16_t, false> ram(0x8000);
		file.read(reinterpret_cast<char*>(ram.GetData()), std::min<size_t>(size, ram.GetSize()));

		return ram;
	}();

	static auto mmu = []
	{
		Common::MMU<uint16_t, false> mmu(0x1000, 16);

		Common::MemoryMapping16 cartridge_mapping;
		cartridge_mapping.SetStart(0);
		cartridge_mapping.SetSize(cartridge.GetSize());
		mmu.Map(&cartridge, cartridge_mapping);

		return mmu;
	}();

	// Initialize cpu
	static auto cpu = []
	{
		Gameboy::CPU cpu(mmu, Gameboy::GameboyType::Classic);
		cpu.Reset();
		return cpu;
	}();

	// Create debugger
	static Gameboy::Debugger debugger(cpu);

	ImGui::ShowDemoWindow();

	// Show debugger
	static bool running = false;
	if (running)
	{
		running = debugger.Run();
	}

	if (ImGui::Begin("Debugger"))
	{
		if (ImGui::Button("Run"))
		{
			running = true;
		}

		ImGui::SameLine();
		if (ImGui::Button("Break"))
		{
			running = false;
		}

		ImGui::SameLine();
		if (ImGui::Button("Step forward") && !running)
		{
			debugger.Step();
		}

		ImGui::SameLine();
		if (ImGui::Button("Reset") && !running)
		{
			debugger.Reset();
		}
	}
	ImGui::End();

	// Show registers
	ImGui::SetNextWindowDockID(dock_id, ImGuiSetCond_FirstUseEver);
	DrawRegisterWindow("Registers", cpu.GetRegisters());

	// Show disassembly

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
