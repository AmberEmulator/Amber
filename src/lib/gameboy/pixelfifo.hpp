#ifndef H_AMBER_GAMEBOY_PIXELFIFO
#define H_AMBER_GAMEBOY_PIXELFIFO

#include <gameboy/api.hpp>
#include <gameboy/pixel.hpp>

namespace Amber::Gameboy
{
	class GAMEBOY_API PixelFIFO
	{
		public:
		size_t GetPixelCount() const noexcept;

		Pixel Pop() noexcept;
		void Push(Pixel a_Pixel) noexcept;
		void Push(const uint8_t a_Colors[2], PixelSource::Enum a_Source) noexcept;
		void Mix(const uint8_t a_Colors[2], PixelSource::Enum a_Source, uint8_t a_Priority) noexcept;

		void Reset();

		private:
		uint64_t m_PixelData = 0;
		size_t m_PixelCount = 7;
	};
}

#endif