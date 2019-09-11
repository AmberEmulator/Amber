#include <gameboy/mmu.hpp>

#include <gameboy/cpu.hpp>
#include <gameboy/joypad.hpp>
#include <gameboy/ppu.hpp>

#include <iostream>

using namespace Amber;
using namespace Common;
using namespace Gameboy;

void MMU::SetBootROM(ROM* a_BootROM)
{
	m_BootROM = a_BootROM;
}

void MMU::SetROM(ROM* a_Rom)
{
	m_ROM = a_Rom;
}

void MMU::SetVRAM(RAM* a_VRAM)
{
	m_VRAM = a_VRAM;
}

void MMU::SetWRAM(RAM* a_WRAM)
{
	m_WRAM = a_WRAM;
}

void MMU::SetCPU(CPU* a_CPU)
{
	m_CPU = a_CPU;
}

void MMU::SetPPU(PPU* a_PPU)
{
	m_PPU = a_PPU;
}

void MMU::SetJoypad(Joypad* a_Joypad)
{
	m_Joypad = a_Joypad;
}

uint8_t MMU::Load8(Address a_Address) const
{
	const uint16_t page = a_Address >> 12;

	switch (page)
	{
		case 0x0:
		if (a_Address <= 0xFF && m_BootROMEnabled && m_BootROM != nullptr)
		{
			return m_BootROM->Load8(a_Address);
		}
		case 0x1:
		case 0x2:
		case 0x3:
		case 0x4:
		case 0x5:
		case 0x6:
		case 0x7:
		if (m_ROM != nullptr)
		{
			return m_ROM->Load8(a_Address);
		}
		break;

		case 0x8:
		case 0x9:
		if (m_VRAM != nullptr)
		{
			return m_VRAM->Load8(a_Address - 0x8000);
		}
		break;

		case 0xC:
		case 0xB:
		if (m_WRAM != nullptr)
		{
			return m_WRAM->Load8(a_Address - 0xC000);
		}
		break;

		case 0xF:
		if (a_Address >= 0xFF80 && a_Address <= 0xFFFE)
		{
			return m_HRAM[a_Address - 0xFF80];
		}
		else if (a_Address == 0xFF44 && m_PPU != nullptr)
		{
			return m_PPU->GetLY();
		}
		else if (a_Address == 0xFF0F && m_CPU != nullptr)
		{
			return m_CPU->GetInterruptRequests();
		}
		else if (a_Address == 0xFFFF && m_CPU != nullptr)
		{
			return m_CPU->GetInterruptEnable();
		}
		else if (a_Address == 0xFF00 && m_Joypad != nullptr)
		{
			return m_Joypad->GetRegister();
		}
		break;
	}

	return 0xFF;
}

void MMU::Store8(Address a_Address, uint8_t a_Value)
{
	const uint16_t page = a_Address >> 12;

	switch (page)
	{
		case 0x0:
		case 0x1:
		case 0x2:
		case 0x3:
		case 0x4:
		case 0x5:
		case 0x6:
		case 0x7:
		if (m_ROM != nullptr)
		{
			m_ROM->Store8(a_Address, a_Value);
		}
		break;

		case 0x8:
		case 0x9:
		if (m_VRAM != nullptr)
		{
			m_VRAM->Store8(a_Address - 0x8000, a_Value);
		}
		break;

		case 0xC:
		case 0xB:
		if (m_WRAM != nullptr)
		{
			m_WRAM->Store8(a_Address - 0xC000, a_Value);
		}
		break;

		case 0xF:
		if (a_Address >= 0xFF80 && a_Address <= 0xFFFE)
		{
			m_HRAM[a_Address - 0xFF80] = a_Value;
		}
		else if (a_Address == 0xFF50)
		{
			m_BootROMEnabled = false;
		}
		else if (a_Address == 0xFF0F && m_CPU != nullptr)
		{
			m_CPU->SetInterruptRequests(a_Value);
		}
		else if (a_Address == 0xFFFF && m_CPU != nullptr)
		{
			m_CPU->SetInterruptEnable(a_Value);
		}
		else if (a_Address == 0xFF01)
		{
			m_TestCharacter = a_Value;
		}
		else if (a_Address == 0xFF02 && a_Value == 0x81)
		{
			std::cout << m_TestCharacter;
		}
		else if (a_Address == 0xFF00 && m_Joypad != nullptr)
		{
			m_Joypad->SetRegister(a_Value);
		}
		break;
	}
}

void MMU::Reset()
{
	m_BootROMEnabled = true;
}