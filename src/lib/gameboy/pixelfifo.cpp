#include <gameboy/pixelfifo.hpp>

#include <array>

using namespace Amber;
using namespace Gameboy;

size_t PixelFIFO::GetPixelCount() const noexcept
{
	return m_PixelCount;
}

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

void PixelFIFO::Push(const uint8_t a_Colors[2], PixelSource::Enum a_Source) noexcept
{
	for (uint8_t i = 0; i < 8; ++i)
	{
		const uint8_t bit0 = (a_Colors[0] >> (7 - i)) & 0b1;
		const uint8_t bit1 = (a_Colors[1] >> (7 - i)) & 0b1;
		const uint8_t color = bit0 | (bit1 << 1);

		Pixel pixel(color, a_Source);
		Push(pixel);
	}
}

void PixelFIFO::Mix(const uint8_t a_Colors[2], PixelSource::Enum a_Source, uint8_t a_Priority) noexcept
{
	// TODO: mix sprite with existing data
}

void PixelFIFO::Reset()
{
	m_PixelData = 0;
	m_PixelCount = 7;
}