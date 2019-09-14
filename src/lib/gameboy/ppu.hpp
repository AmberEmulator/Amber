#ifndef H_AMBER_GAMEBOY_PPU
#define H_AMBER_GAMEBOY_PPU

#include <gameboy/api.hpp>
#include <gameboy/lcdmode.hpp>

namespace Amber::Gameboy
{
	class CPU;
	class MMU;

	class GAMEBOY_API PPU
	{
		public:
		static constexpr uint16_t LCDWidth = 160;
		static constexpr uint16_t LCDHeight = 144;

		static constexpr uint16_t OAMCycles = 80;
		static constexpr uint16_t LineCycles = 456;
		static constexpr uint16_t ScreenLines = LCDHeight;
		static constexpr uint16_t VBlankLines = 10;
		static constexpr uint16_t FrameLines = ScreenLines + VBlankLines;
		static constexpr uint16_t ScreenCycles = LineCycles * ScreenLines;
		static constexpr uint16_t VBlankCycles = LineCycles * VBlankLines;
		static constexpr uint16_t FrameCycles = LineCycles * FrameLines;

		PPU();

		uint8_t GetLY() const noexcept;

		void SetCPU(CPU* a_CPU) noexcept;
		void SetMMU(MMU* a_MMU) noexcept;

		void Tick();
		void Reset();

		void Blit(void* a_Destination, size_t a_Pitch) const noexcept;

		private:
		void GotoOAM() noexcept;

		void OAMSearch() noexcept;
		void PixelTransfer() noexcept;

		uint8_t GetPixel(uint8_t a_X, uint8_t a_Y) const noexcept;
		void SetPixel(uint8_t a_X, uint8_t a_Y, uint8_t a_Color) noexcept;

		// Other components
		CPU* m_CPU = nullptr;
		MMU* m_MMU = nullptr;

		// OAM
		uint8_t m_Sprites[10];
		uint8_t m_SpriteCount;
		uint8_t m_SpriteY;

		// LCD Mode
		uint16_t m_HCounter = 0;
		uint16_t m_VCounter = 0;
		LCDMode::Enum m_LCDMode = LCDMode::VBlank;

		// LCD result buffer
		uint8_t m_LCDBuffer[(LCDWidth * LCDHeight) / 4] = {};
	};
}

#endif