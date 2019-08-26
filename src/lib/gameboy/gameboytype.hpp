#ifndef H_AMBER_GAMEBOY_GAMEBOYTYPE
#define H_AMBER_GAMEBOY_GAMEBOYTYPE

#include <gameboy/api.hpp>

namespace Amber::Gameboy
{
	namespace GameboyType
	{
		enum Enum : uint8_t
		{
			Classic,
			Color,
			Super,
			Super2,
		};
	}
}

#endif