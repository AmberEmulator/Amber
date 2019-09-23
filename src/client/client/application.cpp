#include <client/application.hpp>

#include <client/breakpoints.hpp>
#include <client/disassembly.hpp>
#include <client/gameboywidgets.hpp>
#include <client/texture.hpp>

#include <gameboy/cartridgeloader.hpp>
#include <gameboy/cpu.hpp>
#include <gameboy/debugger.hpp>
#include <gameboy/device.hpp>
#include <gameboy/joypad.hpp>
#include <gameboy/mmu.hpp>
#include <gameboy/ppu.hpp>
#include <gameboy/videoviewer.hpp>

#include <common/ram.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_memory_editor.h>

#include <fstream>
#include <iostream>

using namespace Amber;
using namespace Client;

Application::Application()
{
	//static std::ofstream out("tracelog.txt");
	//std::cout.rdbuf(out.rdbuf());
}

void Application::Tick()
{
	const auto dock_id = ImGui::DockSpaceOverViewport();

	// Load cartridge
	static auto bootrom = []
	{
		std::ifstream boot("C:\\ROMs\\boot.gb", std::ios::binary | std::ios::ate);
		std::streamsize boot_size = boot.tellg();
		boot.seekg(0, std::ios::beg);

		Common::RAM<uint16_t, false> ram(0x100);
		boot.read(reinterpret_cast<char*>(ram.GetData()), std::min<size_t>(boot_size, ram.GetSize()));

		return ram;
	}();

	static auto cartridge = []
	{
		Gameboy::CartridgeLoader loader;

		std::ifstream file("C:\\ROMs\\test.gb", std::ios::binary | std::ios::ate);
		//std::streamsize cartidge_size = cartidge.tellg();
		//cartidge.seekg(0, std::ios::beg);
		//
		//Common::RAM<uint16_t, false> ram(0x8000, 1);
		//cartidge.read(reinterpret_cast<char*>(ram.GetData()), std::min<size_t>(cartidge_size, ram.GetSize()));

		auto cartridge = loader.LoadCartridge(file);
		std::cout << cartridge->GetHeader().GetTitle() << std::endl;
		return cartridge;
	}();


	static Common::RAM<uint16_t, false> vram(0x2000);
	static Common::RAM<uint16_t, false> eram(0x2000);
	static Common::RAM<uint16_t, false> wram(0x2000);
	static Common::RAM<uint16_t, false> pad(0x1000);

	// Initialize device
	static auto device = []
	{
		auto device = std::make_unique<Gameboy::Device>(Gameboy::DeviceDescription::DMG);

		auto& mmu = device->GetMMU();
		mmu.SetBootROM(&bootrom);
		mmu.SetCartridge(cartridge.get());
		mmu.SetVRAM(&vram);
		mmu.SetWRAM(&wram);

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

	static Texture lcd_texture(Gameboy::PPU::LCDWidth, Gameboy::PPU::LCDHeight);
	static const size_t lcd_buffer_size = Gameboy::PPU::LCDWidth * Gameboy::PPU::LCDHeight * 4;
	static const auto lcd_buffer = std::make_unique<uint8_t[]>(lcd_buffer_size);

	device->GetPPU().Blit(lcd_buffer.get(), Gameboy::PPU::LCDWidth);
	lcd_texture.Blit(0, 0, Gameboy::PPU::LCDWidth, Gameboy::PPU::LCDHeight, lcd_buffer.get());


	// Show memory
	static MemoryEditor memory_editor = []
	{
		MemoryEditor memory_editor;
		memory_editor.ReadOnly = true;
		memory_editor.ReadFn = [](const uint8_t* a_Data, size_t a_Offset)
		{
			auto& mmu = *reinterpret_cast<const Gameboy::MMU*>(a_Data);

			return mmu.Load8(static_cast<uint16_t>(a_Offset));
		};

		return memory_editor;
	}();
	if (ImGui::Begin("Memory"))
	{
		if (ImGui::Button("ROM"))
		{
			memory_editor.GotoAddrAndHighlight(0, 0x8000);
		}
		ImGui::SameLine();
		if (ImGui::Button("VRAM"))
		{
			memory_editor.GotoAddrAndHighlight(0x8000, 0xC000);
		}
		ImGui::SameLine();
		if (ImGui::Button("WRAM"))
		{
			memory_editor.GotoAddrAndHighlight(0xC000, 0xE000);
		}
		ImGui::SameLine();
		if (ImGui::Button("OAM"))
		{
			memory_editor.GotoAddrAndHighlight(0xFE00, 0xFEA0);
		}
		ImGui::SameLine();
		if (ImGui::Button("I/O"))
		{
			memory_editor.GotoAddrAndHighlight(0xFF00, 0xFF80);
		}
		ImGui::SameLine();
		if (ImGui::Button("HRAM"))
		{
			memory_editor.GotoAddrAndHighlight(0xFF80, 0xFFFF);
		}

		if (ImGui::Button("PC"))
		{
			const uint16_t pc = device->GetCPU().LoadRegister16(Gameboy::CPU::RegisterPC);
			memory_editor.GotoAddrAndHighlight(pc, pc + 1);
		}
		ImGui::SameLine();
		if (ImGui::Button("SP"))
		{
			const uint16_t sp = device->GetCPU().LoadRegister16(Gameboy::CPU::RegisterSP);
			memory_editor.GotoAddrAndHighlight(sp, sp + 1);
		}

		memory_editor.DrawContents(&(device->GetMMU()), 0x10000);
	}
	ImGui::End();

	// Create debugger
	static Gameboy::Debugger debugger(*device);

	//ImGui::ShowDemoWindow();

	// Show debugger
	static bool running = false;
	if (running)
	{
		running = debugger.Run();
		memory_editor.HighlightMin = device->GetCPU().LoadRegister16(Gameboy::CPU::RegisterPC);
		memory_editor.HighlightMax = memory_editor.HighlightMin + 1;
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
		if (ImGui::ButtonEx("Step", ImVec2(0, 0), ImGuiButtonFlags_Repeat) && !running)
		{
			debugger.Step();
			memory_editor.HighlightMin = device->GetCPU().LoadRegister16(Gameboy::CPU::RegisterPC);
			memory_editor.HighlightMax = memory_editor.HighlightMin + 1;
		}

		ImGui::SameLine();
		if (ImGui::ButtonEx("Microstep", ImVec2(0, 0), ImGuiButtonFlags_Repeat) && !running)
		{
			debugger.Microstep();

			memory_editor.HighlightMin = device->GetCPU().LoadRegister16(Gameboy::CPU::RegisterPC);
			memory_editor.HighlightMax = memory_editor.HighlightMin + 1;
		}

		ImGui::SameLine();
		if (ImGui::Button("Reset"))
		{
			running = debugger.Reset() && running;
		}

		ImGui::Image(reinterpret_cast<ImTextureID>(lcd_texture.GetNativeHandle()), ImVec2(Gameboy::PPU::LCDWidth * 2, Gameboy::PPU::LCDHeight * 2));
		ImGui::Image(reinterpret_cast<ImTextureID>(tile_texture.GetNativeHandle()), ImVec2(tile_texture_width * 2, tile_texture_height * 2));
		ImGui::SameLine();
		ImGui::Text("LY: %i", static_cast<int>(device->GetPPU().GetLY()));
	}
	ImGui::End();

	// Show registers
	ImGui::SetNextWindowDockID(dock_id, ImGuiSetCond_FirstUseEver);
	DrawRegisterWindow("Registers", device->GetCPU());

	// Show joypad
	if (ImGui::Begin("Joypad"))
	{
		auto& joypad = device->GetJoypad();
		uint8_t state = joypad.GetRegister();

		const ImGuiInputTextFlags flags = ImGuiInputTextFlags_NoHorizontalScroll | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsHexadecimal;
		uint32_t value = state;
		if (ImGui::InputScalar("Register", ImGuiDataType_U32, &value, nullptr, nullptr, "%02X", flags))
		{
			state = static_cast<uint8_t>(value);
		}
		bool buttons_selected = (state & Gameboy::Joypad::ButtonSelectMask) == 0;
		if (ImGui::Checkbox("Buttons", &buttons_selected))
		{
			if (buttons_selected)
			{
				state &= ~Gameboy::Joypad::ButtonSelectMask;
			}
			else
			{
				state |= Gameboy::Joypad::ButtonSelectMask;
			}
		}

		ImGui::SameLine(100.0f);
		bool direction_selected = (state & Gameboy::Joypad::DirectionSelectMask) == 0;
		if (ImGui::Checkbox("Directions", &direction_selected))
		{
			if (direction_selected)
			{
				state &= ~Gameboy::Joypad::DirectionSelectMask;
			}
			else
			{
				state |= Gameboy::Joypad::DirectionSelectMask;
			}
		}

		for (uint8_t i = 0; i < 8; ++i)
		{
			const char* names[] = { "Right", "Left", "Up", "Down", "A", "B", "Select", "Start" };
			const char* name = names[i];

			if (i % 2 == 1)
			{
				ImGui::SameLine(100.0f);
			}

			const uint8_t button = 1 << i;
			bool set = joypad.GetButtonState(button) != 0;

			if (ImGui::Checkbox(name, &set))
			{
				joypad.SetButtonState(button, set);
			}
		}

		joypad.SetRegister(state);
	}
	ImGui::End();

	// Show disassembly
	static DisassemblyState disassembly_state;
	disassembly_state.m_Debugger = &debugger;
	disassembly_state.m_ProgramCounter = device->GetCPU().LoadRegister16(Gameboy::CPU::RegisterPC);
	disassembly_state.m_ViewAddress = disassembly_state.m_ProgramCounter;

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
