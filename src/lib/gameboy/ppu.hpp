#ifndef H_AMBER_GAMEBOY_PPU
#define H_AMBER_GAMEBOY_PPU

#include <gameboy/api.hpp>

namespace Amber::Gameboy
{
	class CPU;

	class GAMEBOY_API PPU
	{
		public:
		static constexpr uint16_t LineCycles = 114;
		static constexpr uint16_t ScreenLines = 144;
		static constexpr uint16_t VBlankLines = 10;
		static constexpr uint16_t FrameLines = ScreenLines + VBlankLines;
		static constexpr uint16_t ScreenCycles = LineCycles * ScreenLines;
		static constexpr uint16_t VBlankCycles = LineCycles * VBlankLines;
		static constexpr uint16_t FrameCycles = LineCycles * FrameLines;

		PPU();

		uint8_t GetLY() const noexcept;

		void SetCPU(CPU* a_CPU) noexcept;

		void Tick();
		void Reset();

		private:
		CPU* m_CPU = nullptr;
		uint16_t m_Counter = 0;
	};
}

#endif