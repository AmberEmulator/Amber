#ifndef H_AMBER_GAMEBOY_PIXEL
#define H_AMBER_GAMEBOY_PIXEL

#include <gameboy/api.hpp>
#include <gameboy/pixelsource.hpp>

namespace Amber::Gameboy
{
	class GAMEBOY_API Pixel
	{
		public:
		Pixel() noexcept = default;
		Pixel(uint8_t a_Color, PixelSource::Enum a_Source) noexcept;
		explicit Pixel(uint8_t a_Data) noexcept;
		
		bool IsNull() const noexcept;
		uint8_t GetColor() const noexcept;
		PixelSource::Enum GetSource() const noexcept;
		uint8_t GetData() const noexcept;

		private:
		uint8_t m_Data = 0xFF;
	};
}

#endif