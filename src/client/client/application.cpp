#include <client/application.hpp>

#include <client/breakpoints.hpp>
#include <client/disassembly.hpp>
#include <client/gameboywidgets.hpp>
#include <client/texture.hpp>

#include <gameboy/cpu.hpp>
#include <gameboy/debugger.hpp>
#include <gameboy/registers.hpp>
#include <gameboy/videoviewer.hpp>

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

	static auto mmu = []
	{
		Common::MMU<uint16_t, false> mmu(0x1000, 16);

		Common::MemoryMapping16 rom_mapping;
		rom_mapping.SetStart(0);
		rom_mapping.SetSize(0x8000);
		mmu.Map(&rom, rom_mapping);

		Common::MemoryMapping16 vram_mapping;
		vram_mapping.SetStart(0x8000);
		vram_mapping.SetSize(0x2000);
		mmu.Map(&vram, vram_mapping);

		Common::MemoryMapping16 eram_mapping;
		eram_mapping.SetStart(0xA000);
		eram_mapping.SetSize(0x2000);
		mmu.Map(&eram, eram_mapping);

		Common::MemoryMapping16 wram_mapping;
		wram_mapping.SetStart(0xC000);
		wram_mapping.SetSize(0x2000);
		mmu.Map(&wram, wram_mapping);

		Common::MemoryMapping16 wram_echo_mapping;
		wram_echo_mapping.SetStart(0xE000);
		wram_echo_mapping.SetSize(0x1E00);
		mmu.Map(&wram, wram_echo_mapping);

		Common::MemoryMapping16 pad_mapping;
		pad_mapping.SetStart(0xF000);
		pad_mapping.SetSize(0x1000);
		mmu.Map(&pad, pad_mapping);

		return mmu;
	}();

	// Initialize cpu
	static auto cpu = []
	{
		Gameboy::CPU cpu(mmu/*, Gameboy::GameboyType::Classic*/);
		//cpu.Reset();
		//cpu.GetRegisters().SetPC(0);
		return cpu;
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
		if (ImGui::Button("Step forward") && !running)
		{
			debugger.Step();
		}

		ImGui::SameLine();
		if (ImGui::Button("Reset") && !running)
		{
			debugger.Reset();
			//cpu.GetRegisters().SetPC(0);
		}

		ImGui::Image(reinterpret_cast<ImTextureID>(tile_texture.GetNativeHandle()), ImVec2(tile_texture_width * 2, tile_texture_height * 2));
	}
	ImGui::End();

	// Show registers
	ImGui::SetNextWindowDockID(dock_id, ImGuiSetCond_FirstUseEver);
	DrawRegisterWindow("Registers", cpu);

	// Show disassembly
	static DisassemblyState disassembly_state;
	disassembly_state.m_Debugger = &debugger;
	disassembly_state.m_ViewAddress = cpu.GetPC();

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
