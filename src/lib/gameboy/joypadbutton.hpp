#ifndef H_AMBER_GAMEBOY_JOYPADButton
#define H_AMBER_GAMEBOY_JOYPADButton

#include <gameboy/api.hpp>

namespace Amber::Gameboy
{
	namespace JoypadButton
	{
		enum Enum : uint8_t
		{
			Right  = 0b00000001,
			Left   = 0b00000010,
			Up     = 0b00000100,
			Down   = 0b00001000,
			A      = 0b00010000,
			B      = 0b00100000,
			Select = 0b01000000,
			Start  = 0b10000000,
		};
	}
}

#endif