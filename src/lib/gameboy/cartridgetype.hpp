#ifndef H_AMBER_GAMEBOY_CARTRIDGETYPE
#define H_AMBER_GAMEBOY_CARTRIDGETYPE

#include <gameboy/api.hpp>

namespace Amber::Gameboy
{
	namespace CartridgeType
	{
		enum Enum : uint8_t
		{
			ROM = 0x00,
			ROM_RAM = 0x08,
			ROM_RAM_BATTERY = 0x09,

			MBC1 = 0x01,
			MBC1_RAM = 0x02,
			MBC1_RAM_BATTERY = 0x03,

			// MBC2 without ram doesn't exist since it is part of the MBC itself
			MBC2_RAM = 0x05,
			MBC2_RAM_BATTERY = 0x06,

			MMM01 = 0x0B,
			MMM01_RAM = 0x0C,
			MMM01_RAM_BATTERY = 0x0D,

			MBC3 = 0x11,
			MBC3_RAM = 0x12,
			MBC3_RAM_BATTERY = 0x13,
			MBC3_TIMER_BATTERY = 0x0F,
			MBC3_TIMER_RAM_BATTERY = 0x10,

			MBC4 = 0x15,
			MBC4_RAM = 0x16,
			MBC4_RAM_BATTERY = 0x17,

			MBC5 = 0x19,
			MBC5_RAM = 0x1A,
			MBC5_RAM_BATTERY = 0x1B,
			MBC5_RUMBLE = 0x1C,
			MBC5_RUMBLE_RAM = 0x1D,
			MBC5_RUMBLE_RAM_BATTERY = 0x1E,

			POCKET_CAMERA = 0xFC,
			BANDAI_TAMA5 = 0xFD,

			HuC3 = 0xFE,
			HuC1_RAM_BATTERY = 0xFF,
		};
	}
}

#endif