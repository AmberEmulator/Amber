#include <gameboy/pixel.hpp>

using namespace Amber;
using namespace Gameboy;

Pixel::Pixel(uint8_t a_Color, PixelSource::Enum a_Source) noexcept:
	m_Data((a_Color & 0b11) | ((a_Source & 0b11) << 2))
{
}

Pixel::Pixel(uint8_t a_Data) noexcept:
	m_Data(a_Data)
{
}

bool Pixel::IsNull() const noexcept
{
	return m_Data == 0xFF;
}

uint8_t Pixel::GetColor() const noexcept
{
	return m_Data & 0b11;
}

PixelSource::Enum Pixel::GetSource() const noexcept
{
	return static_cast<PixelSource::Enum>(m_Data >> 2);
}

uint8_t Pixel::GetData() const noexcept
{
	return m_Data;
}