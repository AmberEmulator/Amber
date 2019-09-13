#include <gameboy/cartridgeheader.hpp>

using namespace Amber;
using namespace Gameboy;

std::string_view CartridgeHeader::GetTitle() const noexcept
{
	// Measure the title length
	size_t length = 0;
	for (; length < 16; ++length)
	{
		if (m_Data[TitleOffset + length] == '\0')
		{
			break;
		}
	}

	// Create string_view that points to the title
	return std::string_view(reinterpret_cast<const char*>(m_Data + TitleOffset), length);
}

CartridgeType::Enum CartridgeHeader::GetCartridgeType() const noexcept
{
	return static_cast<CartridgeType::Enum>(m_Data[CartridgeTypeOffset]);
}

size_t CartridgeHeader::GetROMSize() const noexcept
{
	switch (m_Data[ROMSizeOffset])
	{
		case 0x01:
		return 0x10000; // 64 kb, 4 banks

		case 0x02:
		return 0x20000; // 128 kb, 8 banks

		case 0x03:
		return 0x40000; // 256 kb, 16 banks

		case 0x04:
		return 0x80000; // 512 kb, 32 banks

		case 0x05:
		return 0x100000; // 1 mb, 64 banks

		case 0x06:
		return 0x200000; // 2 mb, 128 banks

		case 0x07:
		return 0x400000; // 4 mb, 256 banks

		case 0x52:
		return 0x120000; // 1.1 mb, 72 banks

		case 0x53:
		return 0x140000; // 1.2 mb, 80 banks

		case 0x54:
		return 0x180000; // 1.5 mb, 96 banks

		default:
		// TODO: some sort of error handling?
		case 0x00:
		return 0x8000; // 32 kb, no banking
	}
}

size_t CartridgeHeader::GetRAMSize() const noexcept
{
	switch (m_Data[RAMSizeOffset])
	{
		case 0x01:
		return 0x800; // 2 kb, no banking

		case 0x02:
		return 0x2000; // 8 kb, no banking

		case 0x03:
		return 0x8000; // 32 kb, 4 banks

		case 0x05:
		return 0x10000; // 64 kb, 8 banks

		default:
		// TODO: some sort of error handling?
		case 0x00:
		return 0; // no RAM
	}
}