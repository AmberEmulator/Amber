#include <gameboy/pixelfifo.hpp>

using namespace Amber;
using namespace Gameboy;

Pixel PixelFIFO::Pop() noexcept
{
	if (m_PixelCount > 8)
	{
		Pixel pixel(m_PixelData & 0b1111);
		m_PixelData >>= 4;
		--m_PixelCount;
		return pixel;
	}
	else
	{
		return {};
	}
}

void PixelFIFO::Push(Pixel a_Pixel) noexcept
{
	m_PixelData |= static_cast<uint64_t>(a_Pixel.GetData()) << (m_PixelCount * 4);
	++m_PixelCount;
}

void PixelFIFO::Mix(PixelSource::Enum a_Source, uint8_t a_Priority, uint8_t a_Data[2]) noexcept
{
	// TODO: mix sprite with existing data
}