#ifndef H_AMBER_GAMEBOY_PPUOBSERVER
#define H_AMBER_GAMEBOY_PPUOBSERVER

#include <gameboy/api.hpp>
#include <gameboy/ppu.hpp>

namespace Amber::Gameboy
{
	class GAMEBOY_API PPUObserver
	{
		public:
		virtual ~PPUObserver() noexcept = 0;

		virtual void OnLCDModeChange(LCDMode::Enum a_From, LCDMode::Enum a_To) {};
	};
}

#endif