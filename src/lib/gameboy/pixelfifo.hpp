#ifndef H_AMBER_GAMEBOY_PIXELFIFO
#define H_AMBER_GAMEBOY_PIXELFIFO

#include <gameboy/api.hpp>
#include <gameboy/pixel.hpp>

namespace Amber::Gameboy
{
	class GAMEBOY_API PixelFIFO
	{
		public:
		static constexpr uint8_t PriorityAttributeMask     = 0b1000'0000;
		static constexpr uint8_t YFlipAttributeMask        = 0b0100'0000;
		static constexpr uint8_t XFlipAttributeMask        = 0b0010'0000;
		static constexpr uint8_t PaletteAttributeMask      = 0b0001'0000;
		static constexpr uint8_t VRAMBankAttributeMask     = 0b0000'1000;
		static constexpr uint8_t ColorPaletteAttributeMask = 0b0000'0111;

		size_t GetPixelCount() const noexcept;
		bool IsEmpty() const noexcept;
		bool IsPaused() const noexcept;

		void Push(Pixel a_Pixel) noexcept;
		void Push(const uint8_t a_Colors[2], PixelSource::Enum a_Source) noexcept;
		Pixel Pop() noexcept;

		void SetPaused(bool a_Paused) noexcept;

		Pixel GetPixel(size_t a_Index) const noexcept;
		void SetPixel(size_t a_Index, Pixel a_Pixel) noexcept;

		void MixSprite(const uint8_t a_Colors[2], uint8_t a_Attributes) noexcept;

		void Reset(size_t a_PixelCount);

		private:
		uint64_t m_PixelData = 0;
		size_t m_PixelCount = 0;
		bool m_Paused = false;
	};
}

#endif