#include <gameboy/pixelfifo.hpp>

#include <array>

using namespace Amber;
using namespace Gameboy;

namespace
{
	constexpr std::array<uint8_t, 256> g_ReverseTable = []() constexpr
	{
		std::array<uint8_t, 256> bytes{};
		for (size_t i = 0; i < 256; ++i)
		{
			const uint8_t from = static_cast<uint8_t>(i);
			uint8_t to = 0;

			for (size_t bit = 0; bit < 8; ++bit)
			{
				to <<= 1;
				const uint8_t mask = static_cast<uint8_t>(1 << bit);

				if (from & mask)
				{
					to |= 1;
				}
			}

			bytes[i] = to;
		}

		return bytes;
	}();
}

size_t PixelFIFO::GetPixelCount() const noexcept
{
	return m_PixelCount;
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

Pixel PixelFIFO::GetPixel(size_t a_Index) const noexcept
{
	return Pixel((m_PixelData >> (a_Index * 4)) & 0b1111);
}

void PixelFIFO::SetPixel(size_t a_Index, Pixel a_Pixel) noexcept
{
	const uint64_t mask = ~(0b1111_u64 << (a_Index * 4));
	m_PixelData |= (m_PixelData & mask) | static_cast<uint64_t>(a_Pixel.GetData()) << (a_Index * 4);
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

void PixelFIFO::MixSprite(const uint8_t a_Colors[2], uint8_t a_Attributes) noexcept
{
	uint8_t colors[2];
	std::memcpy(colors, a_Colors, 2);
	if (a_Attributes & XFlipAttributeMask)
	{
		colors[0] = g_ReverseTable[colors[0]];
		colors[1] = g_ReverseTable[colors[1]];
	}

	const PixelSource::Enum source = (a_Attributes & PaletteAttributeMask) ? PixelSource::Sprite1 : PixelSource::Sprite0;

	// TODO: mix sprite with existing data
	for (uint8_t i = 0; i < 8; ++i)
	{
		const Pixel destination = GetPixel(i);

		if (PixelSource::IsSprite(destination.GetSource()))
		{
			continue;
		}

		if (a_Attributes & PriorityAttributeMask)
		{
			if (destination.GetColor() != 0)
			{
				continue;
			}
		}

		const uint8_t bit0 = (colors[0] >> (7 - i)) & 0b1;
		const uint8_t bit1 = (colors[1] >> (7 - i)) & 0b1;
		const uint8_t color = bit0 | (bit1 << 1);
		if (color == 0)
		{
			continue;
		}

		Pixel pixel(color, source);
		SetPixel(i, pixel);
	}
}

void PixelFIFO::Reset()
{
	m_PixelData = 0;
	m_PixelCount = 7;
}