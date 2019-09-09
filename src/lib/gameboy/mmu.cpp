#include <gameboy/mmu.hpp>

using namespace Amber;
using namespace Common;
using namespace Gameboy;

MMU::MMU()
{
}

void MMU::SetROM(ROM* a_Rom)
{
	m_ROM = a_Rom;
}

void MMU::SetVRAM(RAM* a_VRAM)
{
	m_VRAM = a_VRAM;
}

void MMU::SetPPU(PPU* a_PPU)
{
	m_PPU = a_PPU;
}

uint8_t MMU::Load8(Address a_Address) const
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

		case 0xF:
		if (a_Address >= 0xFF80 && a_Address <= 0xFFFE)
		{
			return m_HRAM[a_Address - 0xFF80];
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
			return m_ROM->Store8(a_Address, a_Value);
		}
		break;

		case 0x8:
		case 0x9:
		if (m_VRAM != nullptr)
		{
			return m_VRAM->Store8(a_Address - 0x8000, a_Value);
		}
		break;

		case 0xF:
		if (a_Address >= 0xFF80 && a_Address <= 0xFFFE)
		{
			m_HRAM[a_Address - 0xFF80] = a_Value;
		}
		break;
	}
}