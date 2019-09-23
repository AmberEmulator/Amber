#ifndef H_AMBER_GAMEBOY_LCDMODE
#define H_AMBER_GAMEBOY_LCDMODE

#include <gameboy/api.hpp>

namespace Amber::Gameboy
{
	namespace LCDMode
	{
		enum Enum : uint8_t
		{
			HBlank = 0,
			VBlank = 1,
			OAMSearch = 2,
			PixelTransfer = 3,
		};
	}
}

#endif