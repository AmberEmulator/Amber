#include <gameboy/mbc2cartridge.hpp>

using namespace Amber;
using namespace Gameboy;

MBC2Cartridge::MBC2Cartridge(size_t a_ROMSize):
	BasicCartridge(a_ROMSize, 0)
{
}

uint8_t MBC2Cartridge::Load8(Address a_Address) const
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
		if (m_RAMEnabled && a_Address < 0xA200)
		{
			const uint16_t ram_address = (a_Address - 0xA000) >> 1;

			if (a_Address & 1)
			{
				return m_RAM[ram_address] >> 4;
			}
			else
			{
				return m_RAM[ram_address] & 0b1111;
			}
		}
		break;
	}

	return 0xFF;
}

void MBC2Cartridge::Store8(Address a_Address, uint8_t a_Value)
{
	switch (a_Address & 0xF000)
	{
		case 0x0000:
		case 0x1000:
		if ((a_Address & 0b0000'0001'0000'0000) != 0)
		{
			m_RAMEnabled = (a_Value & 0x0A) == 0x0A;
		}
		break;

		case 0x2000:
		case 0x3000:
		if ((a_Address & 0b0000'0001'0000'0000) == 0)
		{
			uint8_t bits = a_Value & 0b0000'1111;
			if (bits == 0)
			{
				bits = 1;
			}
			m_ROMBank = (m_ROMBank & 0b1111'0000) | bits;
		}
		break;

		case 0xA000:
		if (m_RAMEnabled && a_Address < 0xA200)
		{
			const uint16_t ram_address = (a_Address - 0xA000) >> 1;

			if (a_Address & 1)
			{
				m_RAM[ram_address] = (m_RAM[ram_address] & 0b1111'0000) | (a_Value & 0b0000'1111);
			}
			else
			{
				m_RAM[ram_address] = (m_RAM[ram_address] & 0b0000'1111) | (a_Value << 4);
			}
		}
		break;
	}
}