#ifndef H_AMBER_GAMEBOY_PIXELFIFO
#define H_AMBER_GAMEBOY_PIXELFIFO

#include <gameboy/api.hpp>
#include <gameboy/pixel.hpp>

namespace Amber::Gameboy
{
	class GAMEBOY_API PixelFIFO
	{
		public:
		Pixel Pop() noexcept;
		void Push(Pixel a_Pixel) noexcept;
		void Mix(PixelSource::Enum a_Source, uint8_t a_Priority, uint8_t a_Data[2]) noexcept;

		private:
		uint64_t m_PixelData;
		size_t m_PixelCount = 0;
	};
}

#endif