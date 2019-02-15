#ifndef H_DEMU_GAMEBOY_GAMEBOYTYPE
#define H_DEMU_GAMEBOY_GAMEBOYTYPE

#include <gameboy/api.hpp>

namespace Demu::Gameboy
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