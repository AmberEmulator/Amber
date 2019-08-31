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
	for (size_t y = 0; y < GetTileHeight(); ++y)
	{
		for (size_t x = 0; x < GetTileWidth(); ++x)
		{
			uint8_t* const pixel = reinterpret_cast<uint8_t*>(a_Destination) + (y * a_Pitch + x) * 4;

			std::memset(pixel, 0, 4);
			pixel[a_Tile % 3] = 0xFF;
			pixel[3] = 0xFF;
		}
	}
}