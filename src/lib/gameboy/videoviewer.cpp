#include <gameboy/videoviewer.hpp>

using namespace Amber;
using namespace Gameboy;

VideoViewer::VideoViewer(const Common::RAM16<false>& a_VRAM):
	m_VRAM(a_VRAM)
{
}

size_t VideoViewer::GetTileCount() const noexcept
{
	return m_VRAM.GetBankCount() * 192;
}

size_t VideoViewer::GetTileWidth() const noexcept
{
	return 8;
}

void VideoViewer::BlitTile(size_t a_Tile, void* a_Destination, size_t a_Pitch) const
{
	static constexpr uint8_t colors[] = { 0xFF, 0x77, 0xCC, 0x00 };

	uint8_t tile_data[16];
	for (size_t i = 0; i < 16; ++i)
	{
		tile_data[i] = m_VRAM.Load8(a_Tile * 16 + i);
	}

	for (size_t y = 0; y < GetTileHeight(); ++y)
	{
		for (size_t x = 0; x < GetTileWidth(); ++x)
		{
			const uint8_t line = y * 2;

			const uint8_t byte0 = tile_data[line + 0];
			const uint8_t byte1 = tile_data[line + 1];

			const uint8_t bit0 = (byte0 >> (7 - x)) & 0x01;
			const uint8_t bit1 = (byte1 >> (7 - x)) & 0x01;

			const uint8_t color = colors[bit0 | (bit1 << 1)];

			uint8_t* const pixel = reinterpret_cast<uint8_t*>(a_Destination) + (y * a_Pitch + x) * 4;

			pixel[0] = color;
			pixel[1] = color;
			pixel[2] = color;
			pixel[3] = 0xFF;
		}
	}
}