#ifndef H_AMBER_GAMEBOY_PPU
#define H_AMBER_GAMEBOY_PPU

#include <gameboy/api.hpp>
#include <gameboy/lcdmode.hpp>
#include <gameboy/pixelfifo.hpp>
#include <gameboy/tilefetcher.hpp>

namespace Amber::Gameboy
{
	class CPU;
	class MMU;

	class GAMEBOY_API PPU
	{
		public:
		static constexpr size_t LCDWidth = 160;
		static constexpr size_t LCDHeight = 144;

		static constexpr size_t OAMCycles = 80;
		static constexpr size_t LineCycles = 456;
		static constexpr size_t ScreenLines = LCDHeight;
		static constexpr size_t VBlankLines = 10;
		static constexpr size_t FrameLines = ScreenLines + VBlankLines;
		static constexpr size_t ScreenCycles = LineCycles * ScreenLines;
		static constexpr size_t VBlankCycles = LineCycles * VBlankLines;
		static constexpr size_t FrameCycles = LineCycles * FrameLines;

		PPU(MMU& a_MMU);

		uint8_t GetLY() const noexcept;
		uint8_t GetLCDC() const noexcept;
		uint8_t GetSTAT() const noexcept;
		uint8_t GetSCX() const noexcept;
		uint8_t GetSCY() const noexcept;
		uint8_t GetBGP() const noexcept;
		uint8_t GetOBP0() const noexcept;
		uint8_t GetOBP1() const noexcept;

		uint8_t* GetOAM() noexcept;
		const uint8_t* GetOAM() const noexcept;

		void SetCPU(CPU* a_CPU) noexcept;
		void SetLCDC(uint8_t a_Value) noexcept;
		void SetSTAT(uint8_t a_Value) noexcept;
		void SetSCX(uint8_t a_Value) noexcept;
		void SetSCY(uint8_t a_Value) noexcept;
		void SetBGP(uint8_t a_Value) noexcept;
		void SetOBP0(uint8_t a_Value) noexcept;
		void SetOBP1(uint8_t a_Value) noexcept;

		void Tick();
		void Reset();

		void Blit(void* a_Destination, size_t a_Pitch) const noexcept;

		private:
		struct SpriteDrawInfo
		{
			uint8_t m_SpriteIndex;
			uint8_t m_TileY;
			uint8_t m_DrawX;
			uint8_t m_Attributes;
		};

		void GotoOAM() noexcept;
		void GotoPixelTransfer() noexcept;

		void OAMSearch() noexcept;
		void PixelTransfer() noexcept;

		uint8_t GetPixel(uint8_t a_X, uint8_t a_Y) const noexcept;
		void SetPixel(uint8_t a_X, uint8_t a_Y, uint8_t a_Color) noexcept;

		// Other components
		MMU& m_MMU;
		CPU* m_CPU = nullptr;

		// OAM
		uint8_t m_OAM[160];
		SpriteDrawInfo m_Sprites[10];
		uint8_t m_SpriteCount;
		uint8_t m_SpriteY;
		uint8_t m_CurrentSprite;
		uint8_t m_SpriteAttributes;

		// LCD mode
		uint16_t m_HCounter = 0;
		uint16_t m_VCounter = 0;
		LCDMode::Enum m_LCDMode = LCDMode::VBlank;

		// LCD control
		uint8_t m_LCDC = 0x91;
		uint8_t m_STAT = 0x00;
		uint8_t m_SCX = 0x00;
		uint8_t m_SCY = 0x00;

		// Palettes
		uint8_t m_BGP = 0b11100100;
		uint8_t m_OBP0 = 0b11100100;
		uint8_t m_OBP1 = 0b11100100;

		// Drawing
		PixelFIFO m_PixelFIFO;
		TileFetcher m_TileFetcher;
		uint8_t m_DrawX;

		// LCD result buffer
		uint8_t m_LCDBuffer[(LCDWidth * LCDHeight) / 4] = {};
	};
}

#endif