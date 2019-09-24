#include <gameboy/mmu.hpp>

#include <gameboy/cpu.hpp>
#include <gameboy/joypad.hpp>
#include <gameboy/ppu.hpp>

#include <iostream>

using namespace Amber;
using namespace Common;
using namespace Gameboy;

MMU::MMU()
{
	for (uint8_t i = 0; i < 0xF; ++i)
	{
		m_PageLoads[i] = &MMU::LoadNOP;
		m_PageStores[i] = &MMU::StoreNOP;
	}

	m_PageLoads[0xF] = &MMU::LoadLastPage;
	m_PageStores[0xF] = &MMU::StoreLastPage;

	for (size_t i = 0; i < 512; ++i)
	{
		m_LastLoads[i] = &MMU::LoadNOP;
		m_LastStores[i] = &MMU::StoreNOP;
	}

	for (size_t i = 0x180; i < 0x1FF; ++i)
	{
		m_LastLoads[i] = &MMU::LoadArray<&MMU::m_HRAM, 0xFF80>;
		m_LastStores[i] = &MMU::StoreArray<&MMU::m_HRAM, 0xFF80>;
	}
}

void MMU::SetBootROM(Memory* a_BootROM)
{
	m_BootROM = a_BootROM;
	if (m_BootROM != nullptr)
	{
		m_PageLoads[0x0] = &MMU::LoadBoot;
		m_PageStores[0x0] = &MMU::StoreBoot;

		m_LastStores[0x0150] = &MMU::StoreDisableBoot;
	}
	else
	{
		if (m_Cartridge != nullptr)
		{
			m_PageLoads[0x0] = &MMU::LoadMemory<&MMU::m_Cartridge, 0>;
			m_PageStores[0x0] = &MMU::StoreMemory<&MMU::m_Cartridge, 0>;
		}
		else
		{
			m_PageLoads[0x0] = &MMU::LoadNOP;
			m_PageStores[0x0] = &MMU::StoreNOP;
		}

		m_LastStores[0x0150] = &MMU::StoreNOP;
	}
}

void MMU::SetCartridge(Memory* a_Cartridge)
{
	m_Cartridge = a_Cartridge;
	if (m_Cartridge != nullptr)
	{
		// ROM
		for (uint8_t i = 0x0; i < 0x8; ++i)
		{
			m_PageLoads[i] = &MMU::LoadMemory<&MMU::m_Cartridge, 0>;
			m_PageStores[i] = &MMU::StoreMemory<&MMU::m_Cartridge, 0>;
		}

		// RAM
		for (uint8_t i = 0xA; i < 0xC; ++i)
		{
			m_PageLoads[i] = &MMU::LoadMemory<&MMU::m_Cartridge, 0>;
			m_PageStores[i] = &MMU::StoreMemory<&MMU::m_Cartridge, 0>;
		}
	}
	else
	{
		// ROM
		for (uint8_t i = 0x0; i < 0x8; ++i)
		{
			m_PageLoads[i] = &MMU::LoadNOP;
			m_PageStores[i] = &MMU::StoreNOP;
		}

		// RAM
		for (uint8_t i = 0xA; i < 0xC; ++i)
		{
			m_PageLoads[i] = &MMU::LoadNOP;
			m_PageStores[i] = &MMU::StoreNOP;
		}
	}

	SetBootROM(m_BootROM);
}

void MMU::SetVRAM(Memory* a_VRAM)
{
	m_VRAM = a_VRAM;
	if (m_VRAM != nullptr)
	{
		for (uint8_t i = 0x8; i < 0xA; ++i)
		{
			m_PageLoads[i] = &MMU::LoadMemory<&MMU::m_VRAM, 0x8000>;
			m_PageStores[i] = &MMU::StoreMemory<&MMU::m_VRAM, 0x8000>;
		}
	}
	else
	{
		for (uint8_t i = 0x8; i < 0xA; ++i)
		{
			m_PageLoads[i] = &MMU::LoadNOP;
			m_PageStores[i] = &MMU::StoreNOP;
		}
	}
}

void MMU::SetWRAM(Memory* a_WRAM)
{
	m_WRAM = a_WRAM;
	if (m_WRAM != nullptr)
	{
		for (uint8_t i = 0xC; i < 0xE; ++i)
		{
			m_PageLoads[i] = &MMU::LoadMemory<&MMU::m_WRAM, 0xC000>;
			m_PageStores[i] = &MMU::StoreMemory<&MMU::m_WRAM, 0xC000>;
		}

		m_PageLoads[0xE] = &MMU::LoadMemory<&MMU::m_WRAM, 0xE000>;
		m_PageStores[0xE] = &MMU::StoreMemory<&MMU::m_WRAM, 0xE000>;
	}
	else
	{
		for (uint8_t i = 0xC; i < 0xF; ++i)
		{
			m_PageLoads[i] = &MMU::LoadNOP;
			m_PageStores[i] = &MMU::StoreNOP;
		}
	}
}

void MMU::SetCPU(CPU* a_CPU)
{
	m_CPU = a_CPU;
	if (m_CPU != nullptr)
	{
		m_LastLoads[0x0104] = &MMU::LoadRegister<&MMU::m_CPU, &CPU::GetDIV>;
		m_LastLoads[0x0105] = &MMU::LoadRegister<&MMU::m_CPU, &CPU::GetTIMA>;
		m_LastLoads[0x0106] = &MMU::LoadRegister<&MMU::m_CPU, &CPU::GetTMA>;
		m_LastLoads[0x0107] = &MMU::LoadRegister<&MMU::m_CPU, &CPU::GetTAC>;
		m_LastLoads[0x010F] = &MMU::LoadRegister<&MMU::m_CPU, &CPU::GetInterruptRequests>;
		m_LastLoads[0x01FF] = &MMU::LoadRegister<&MMU::m_CPU, &CPU::GetInterruptEnable>;
		
		m_LastStores[0x0104] = &MMU::StoreRegister<&MMU::m_CPU, &CPU::SetDIV>;
		m_LastStores[0x0105] = &MMU::StoreRegister<&MMU::m_CPU, &CPU::SetTIMA>;
		m_LastStores[0x0106] = &MMU::StoreRegister<&MMU::m_CPU, &CPU::SetTMA>;
		m_LastStores[0x0107] = &MMU::StoreRegister<&MMU::m_CPU, &CPU::SetTAC>;
		m_LastStores[0x010F] = &MMU::StoreRegister<&MMU::m_CPU, &CPU::SetInterruptRequests>;
		m_LastStores[0x0146] = &MMU::StoreRegister<&MMU::m_CPU, &CPU::StartDMA>;
		m_LastStores[0x01FF] = &MMU::StoreRegister<&MMU::m_CPU, &CPU::SetInterruptEnable>;
	}
	else
	{
		m_LastLoads[0x0104] = &MMU::LoadNOP;
		m_LastLoads[0x0105] = &MMU::LoadNOP;
		m_LastLoads[0x0106] = &MMU::LoadNOP;
		m_LastLoads[0x0107] = &MMU::LoadNOP;
		m_LastLoads[0x010F] = &MMU::LoadNOP;
		m_LastLoads[0x01FF] = &MMU::LoadNOP;

		m_LastStores[0x0104] = &MMU::StoreNOP;
		m_LastStores[0x0105] = &MMU::StoreNOP;
		m_LastStores[0x0106] = &MMU::StoreNOP;
		m_LastStores[0x0107] = &MMU::StoreNOP;
		m_LastStores[0x010F] = &MMU::StoreNOP;
		m_LastStores[0x0146] = &MMU::StoreNOP;
		m_LastStores[0x01FF] = &MMU::StoreNOP;
	}
}

void MMU::SetPPU(PPU* a_PPU)
{
	m_PPU = a_PPU;
	if (m_PPU != nullptr)
	{
		m_LastLoads[0x0140] = &MMU::LoadRegister<&MMU::m_PPU, &PPU::GetLCDC>;
		m_LastLoads[0x0141] = &MMU::LoadRegister<&MMU::m_PPU, &PPU::GetSTAT>;
		m_LastLoads[0x0142] = &MMU::LoadRegister<&MMU::m_PPU, &PPU::GetSCY>;
		m_LastLoads[0x0143] = &MMU::LoadRegister<&MMU::m_PPU, &PPU::GetSCX>;
		m_LastLoads[0x0144] = &MMU::LoadRegister<&MMU::m_PPU, &PPU::GetLY>;
		m_LastLoads[0x0147] = &MMU::LoadRegister<&MMU::m_PPU, &PPU::GetBGP>;
		m_LastLoads[0x0148] = &MMU::LoadRegister<&MMU::m_PPU, &PPU::GetOBP0>;
		m_LastLoads[0x0149] = &MMU::LoadRegister<&MMU::m_PPU, &PPU::GetOBP1>;
		
		m_LastStores[0x0140] = &MMU::StoreRegister<&MMU::m_PPU, &PPU::SetLCDC>;
		m_LastStores[0x0141] = &MMU::StoreRegister<&MMU::m_PPU, &PPU::SetSTAT>;
		m_LastStores[0x0142] = &MMU::StoreRegister<&MMU::m_PPU, &PPU::SetSCY>;
		m_LastStores[0x0143] = &MMU::StoreRegister<&MMU::m_PPU, &PPU::SetSCX>;
		m_LastStores[0x0147] = &MMU::StoreRegister<&MMU::m_PPU, &PPU::SetBGP>;
		m_LastStores[0x0148] = &MMU::StoreRegister<&MMU::m_PPU, &PPU::SetOBP0>;
		m_LastStores[0x0149] = &MMU::StoreRegister<&MMU::m_PPU, &PPU::SetOBP1>;

		m_OAM = m_PPU->GetOAM();
		for (size_t i = 0; i < 160; ++i)
		{
			m_LastLoads[i] = &MMU::LoadArray<&MMU::m_OAM, 0xFE00>;
			m_LastStores[i] = &MMU::StoreArray<&MMU::m_OAM, 0xFE00>;
		}
	}
	else
	{
		m_LastLoads[0x0140] = &MMU::LoadNOP;
		m_LastLoads[0x0141] = &MMU::LoadNOP;
		m_LastLoads[0x0142] = &MMU::LoadNOP;
		m_LastLoads[0x0143] = &MMU::LoadNOP;
		m_LastLoads[0x0144] = &MMU::LoadNOP;
		m_LastLoads[0x0147] = &MMU::LoadNOP;
		m_LastLoads[0x0148] = &MMU::LoadNOP;
		m_LastLoads[0x0149] = &MMU::LoadNOP;

		m_LastStores[0x0140] = &MMU::StoreNOP;
		m_LastStores[0x0141] = &MMU::StoreNOP;
		m_LastStores[0x0142] = &MMU::StoreNOP;
		m_LastStores[0x0143] = &MMU::StoreNOP;
		m_LastStores[0x0147] = &MMU::StoreNOP;
		m_LastStores[0x0148] = &MMU::StoreNOP;
		m_LastStores[0x0149] = &MMU::StoreNOP;

		m_OAM = nullptr;
		for (size_t i = 0; i < 160; ++i)
		{
			m_LastLoads[i] = &MMU::LoadNOP;
			m_LastStores[i] = &MMU::StoreNOP;
		}
	}
}

void MMU::SetJoypad(Joypad* a_Joypad)
{
	m_Joypad = a_Joypad;
	if (m_Joypad != nullptr)
	{
		m_LastLoads[0x0100] = &MMU::LoadRegister<&MMU::m_Joypad, &Joypad::GetRegister>;
		m_LastStores[0x0100] = &MMU::StoreRegister<&MMU::m_Joypad, & Joypad::SetRegister>;
	}
	else
	{
		m_LastLoads[0x0100] = &MMU::LoadNOP;
		m_LastStores[0x0100] = &MMU::StoreNOP;
	}
}

uint8_t MMU::Load8(Address a_Address) const
{
	const uint16_t page = a_Address >> 12;
	return (this->*(m_PageLoads[page]))(a_Address);
}

void MMU::Store8(Address a_Address, uint8_t a_Value)
{
	const uint16_t page = a_Address >> 12;
	(this->*(m_PageStores[page]))(a_Address, a_Value);
}

void MMU::Reset()
{
	SetBootROM(m_BootROM);
}

uint8_t MMU::LoadNOP(uint16_t a_Address) const
{
	return 0xFF;
}

uint8_t MMU::LoadBoot(uint16_t a_Address) const
{
	if (a_Address <= 0xFF)
	{
		return LoadMemory<&MMU::m_BootROM, 0>(a_Address);
	}
	else if (m_Cartridge != nullptr)
	{
		return LoadMemory<&MMU::m_Cartridge, 0>(a_Address);
	}
	else
	{
		return LoadNOP(a_Address);
	}
}

uint8_t MMU::LoadLastPage(uint16_t a_Address) const
{
	if ((a_Address & 0xFE00) != 0xFE00)
	{
		if (m_WRAM != nullptr)
		{
			return LoadMemory<&MMU::m_WRAM, 0xE000>(a_Address);
		}
		else
		{
			return LoadNOP(a_Address);
		}
	}
	else
	{
		return (this->*(m_LastLoads[a_Address & 0x1FF]))(a_Address);
	}
}

void MMU::StoreNOP(uint16_t a_Address, uint8_t a_Value)
{
}

void MMU::StoreBoot(uint16_t a_Address, uint8_t a_Value)
{
	if (a_Address <= 0xFF)
	{
		StoreMemory<&MMU::m_BootROM, 0>(a_Address, a_Value);
	}
	else if (m_Cartridge != nullptr)
	{
		StoreMemory<&MMU::m_Cartridge, 0>(a_Address, a_Value);
	}
	else
	{
		StoreNOP(a_Address, a_Value);
	}
}

void MMU::StoreDisableBoot(uint16_t a_Address, uint8_t a_Value)
{
	if (m_Cartridge != nullptr)
	{
		m_PageLoads[0x0] = &MMU::LoadMemory<&MMU::m_Cartridge, 0>;
		m_PageStores[0x0] = &MMU::StoreMemory<&MMU::m_Cartridge, 0>;
	}
	else
	{
		m_PageLoads[0x0] = &MMU::LoadNOP;
		m_PageStores[0x0] = &MMU::StoreNOP;
	}
}

void MMU::StoreLastPage(uint16_t a_Address, uint8_t a_Value)
{
	if ((a_Address & 0xFE00) != 0xFE00)
	{
		if (m_WRAM != nullptr)
		{
			StoreMemory<&MMU::m_WRAM, 0xE000>(a_Address, a_Value);
		}
	}
	else
	{
		(this->*(m_LastStores[a_Address & 0x1FF]))(a_Address, a_Value);
	}
}