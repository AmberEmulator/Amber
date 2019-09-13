#include <gameboy/mbc1cartridge.hpp>

using namespace Amber;
using namespace Gameboy;

MBC1Cartridge::MBC1Cartridge(size_t a_ROMSize, size_t a_RAMSize):
	BasicCartridge(a_ROMSize, a_RAMSize)
{
}

uint8_t MBC1Cartridge::Load8(Address a_Address) const
{
	switch (a_Address & 0xF000)
	{
		case 0x0000:
		case 0x1000:
		case 0x2000:
		case 0x3000:
		return m_ROM.Load8(a_Address);

		case 0x4000:
		case 0x5000:
		case 0x6000:
		case 0x7000:
		return m_ROM.Load8((a_Address - 0x4000) + m_ROMBank * ROMBankSize);

		case 0xA000:
		case 0xB000:
		if (m_RAMEnabled)
		{
			return m_RAM.Load8((a_Address - 0xA000) + m_RAMBank * RAMBankSize);
		}
	}

	return 0xFF;
}

void MBC1Cartridge::Store8(Address a_Address, uint8_t a_Value)
{
	switch (a_Address & 0xF000)
	{
		case 0x0000:
		case 0x1000:
		m_RAMEnabled = (a_Value & 0x0A) == 0x0A;
		break;

		case 0x2000:
		case 0x3000:
		{
			uint8_t bits = a_Value & 0b0001'1111;
			if (bits == 0)
			{
				bits = 1;
			}
			m_ROMBank = (m_ROMBank & 0b1110'000) | bits;
		}
		break;

		case 0x4000:
		case 0x5000:
		if (m_ROMBanking)
		{
			m_ROMBank = (m_ROMBank & 0b1001'1111) & ((a_Value & 0b0000'0011) << 5);
		}
		else
		{
			m_RAMBank = a_Value & 0b0000'0011;
		}
		break;

		case 0x6000:
		case 0x7000:
		if (m_ROMBanking = (a_Value & 0x01) == 0)
		{
			m_RAMBank = 0x00;
		}
		else
		{
			m_ROMBank &= 0b0001'1111;
		}
		break;

		case 0xA000:
		case 0xB000:
		if (m_RAMEnabled)
		{
			m_RAM.Store8((a_Address - 0xA000) + m_RAMBank * RAMBankSize, a_Value);
		}
		break;
	}
}