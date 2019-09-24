#ifndef H_AMBER_GAMEBOY_PIXELSOURCE
#define H_AMBER_GAMEBOY_PIXELSOURCE

#include <gameboy/api.hpp>

namespace Amber::Gameboy
{
	namespace PixelSource
	{
		enum Enum : uint8_t
		{
			Background = 0b00,
			Window     = 0b01,
			Sprite0    = 0b10,
			Sprite1    = 0b11
		};

		constexpr bool IsBackground(PixelSource::Enum a_Value)
		{
			return (a_Value & 0b01) != 0;
		}

		constexpr bool IsSprite(PixelSource::Enum a_Value)
		{
			return (a_Value & 0b10) != 0;
		}
	}
}

#endif