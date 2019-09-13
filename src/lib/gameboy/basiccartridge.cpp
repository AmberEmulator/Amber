#include <gameboy/basiccartridge.hpp>

using namespace Amber;
using namespace Gameboy;

BasicCartridge::BasicCartridge(size_t a_ROMSize, size_t a_RAMSize):
	m_ROM(ROMBankSize, a_ROMSize / ROMBankSize),
	m_RAM(RAMBankSize, a_RAMSize / RAMBankSize)
{
}

const CartridgeHeader& BasicCartridge::GetHeader() const
{
	return *reinterpret_cast<const CartridgeHeader*>(m_ROM.GetData() + CartridgeHeader::HeaderAddress);
}

Common::ROM16<false>& BasicCartridge::GetROM() noexcept
{
	return m_ROM;
}

Common::RAM16<false>& BasicCartridge::GetRAM() noexcept
{
	return m_RAM;
}

uint8_t BasicCartridge::Load8(Address a_Address) const
{
	switch (a_Address & 0xF000)
	{
		case 0x0000:
		case 0x1000:
		case 0x2000:
		case 0x3000:
		case 0x4000:
		case 0x5000:
		case 0x6000:
		case 0x7000:
		return m_ROM.Load8(a_Address);

		case 0xA000:
		case 0xB000:
		return m_RAM.Load8(a_Address - 0xA000);
		
		default:
		return 0xFF;
	}
}

void BasicCartridge::Store8(Address a_Address, uint8_t a_Value)
{
	switch (a_Address & 0xF000)
	{
		case 0xA000:
		case 0xB000:
		m_ROM.Store8(a_Address - 0xA000, a_Value);
		break;
	}
}