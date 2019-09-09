#include <client/application.hpp>

#include <client/breakpoints.hpp>
#include <client/disassembly.hpp>
#include <client/gameboywidgets.hpp>
#include <client/texture.hpp>

#include <gameboy/cpu.hpp>
#include <gameboy/debugger.hpp>
#include <gameboy/device.hpp>
#include <gameboy/mmu.hpp>
#include <gameboy/videoviewer.hpp>

#include <common/ram.hpp>

#include <imgui/imgui.h>

#include <fstream>

using namespace Amber;
using namespace Client;

void Application::Tick()
{
	const auto dock_id = ImGui::DockSpaceOverViewport();

	// Load cartridge
	static auto rom = []
	{
		std::ifstream boot("C:\\ROMs\\boot.gb", std::ios::binary | std::ios::ate);
		std::streamsize boot_size = boot.tellg();
		boot.seekg(0, std::ios::beg);

		std::ifstream cartidge("C:\\ROMs\\test.gb", std::ios::binary | std::ios::ate);
		std::streamsize cartidge_size = cartidge.tellg();
		cartidge.seekg(0, std::ios::beg);

		Common::RAM<uint16_t, false> ram(0x8000, 1);
		cartidge.read(reinterpret_cast<char*>(ram.GetData()), std::min<size_t>(cartidge_size, ram.GetSize()));
		boot.read(reinterpret_cast<char*>(ram.GetData()), std::min<size_t>(boot_size, ram.GetSize()));

		return ram;
	}();

	static Common::RAM<uint16_t, false> vram(0x2000, 1);
	static Common::RAM<uint16_t, false> eram(0x2000, 1);
	static Common::RAM<uint16_t, false> wram(0x2000, 1);
	static Common::RAM<uint16_t, false> pad(0x1000, 1);

	// Initialize device
	static auto device = []
	{
		auto device = std::make_unique<Gameboy::Device>(Gameboy::DeviceDescription::DMG);

		auto& mmu = device->GetMMU();
		mmu.SetROM(&rom);
		mmu.SetVRAM(&vram);

		return device;
	}();

	static Gameboy::VideoViewer video_viewer(vram);

	static const size_t tile_columns = 16;
	static const size_t tile_rows = video_viewer.GetTileCount() / tile_columns;
	static const size_t tile_width = video_viewer.GetTileWidth();
	static const size_t tile_height = video_viewer.GetTileHeight();
	static const size_t tile_texture_width = tile_columns * tile_width;
	static const size_t tile_texture_height = tile_rows * tile_height;

	static Texture tile_texture(tile_texture_width, tile_texture_height);

	static const size_t tile_buffer_size = tile_texture_width * tile_texture_height * 4;
	static const auto tile_buffer = std::make_unique<uint8_t[]>(tile_buffer_size);

	for (size_t row = 0; row < tile_rows; ++row)
	{
		for (size_t column = 0; column < tile_columns; ++column)
		{
			const size_t tile = column + row * tile_columns;
			uint8_t* const destination = tile_buffer.get() + (column * tile_width + row * tile_width * tile_height * tile_columns) * 4;

			video_viewer.BlitTile(tile, destination, tile_texture_width);
		}
	}

	tile_texture.Blit(0, 0, tile_texture_width, tile_texture_height, tile_buffer.get());

	// Create debugger
	static Gameboy::Debugger debugger(*device);

	ImGui::ShowDemoWindow();

	// Show debugger
	static bool running = false;
	if (running)
	{
		running = debugger.Run();
	}

	if (ImGui::Begin("Debugger"))
	{
		if (!running)
		{
			if (ImGui::Button("Run"))
			{
				running = true;
			}
		}
		else
		{
			if (ImGui::Button("Break"))
			{
				running = false;
			}
		}

		ImGui::SameLine();
		if (ImGui::Button("Step") && !running)
		{
			const uint16_t pc = device->GetCPU().LoadRegister16(Gameboy::CPU::RegisterPC);
			do
			{
				debugger.Step();
			}
			while (pc == device->GetCPU().LoadRegister16(Gameboy::CPU::RegisterPC));
		}

		ImGui::SameLine();
		if (ImGui::Button("Microstep") && !running)
		{
			debugger.Microstep();
		}

		ImGui::SameLine();
		if (ImGui::Button("Reset"))
		{
			debugger.Reset();
			//cpu.GetRegisters().SetPC(0);
		}

		ImGui::Image(reinterpret_cast<ImTextureID>(tile_texture.GetNativeHandle()), ImVec2(tile_texture_width * 2, tile_texture_height * 2));
	}
	ImGui::End();

	// Show registers
	ImGui::SetNextWindowDockID(dock_id, ImGuiSetCond_FirstUseEver);
	DrawRegisterWindow("Registers", device->GetCPU());

	// Show disassembly
	static DisassemblyState disassembly_state;
	disassembly_state.m_Debugger = &debugger;
	disassembly_state.m_ViewAddress = device->GetCPU().LoadRegister16(Gameboy::CPU::RegisterPC);

	ImGui::SetNextWindowDockID(dock_id, ImGuiSetCond_FirstUseEver);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	if (ImGui::Begin("Disassembly"))
	{
		ShowDisassembly("disassembly", disassembly_state);
	}
	ImGui::PopStyleVar();
	ImGui::End();


	// Show breakpoints
	static BreakpointsState breakpoints_state;
	breakpoints_state.m_Debugger = &debugger;

	ImGui::SetNextWindowDockID(dock_id, ImGuiSetCond_FirstUseEver);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	if (ImGui::Begin("Breakpoints"))
	{
		ImGui::PopStyleVar();
		ShowBreakpoints("breakpoints", breakpoints_state);
	}
	else
	{
		ImGui::PopStyleVar();
	}
	
	ImGui::End();
}
