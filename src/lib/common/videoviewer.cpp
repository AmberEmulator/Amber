#include <common/videoviewer.hpp>

using namespace Amber;
using namespace Common;

VideoViewer::~VideoViewer() noexcept = default;

size_t VideoViewer::GetTileHeight() const noexcept
{
	return GetTileWidth();
}

void VideoViewer::BlitTile(size_t a_Tile, void* a_Destination) const
{
	BlitTile(a_Tile, a_Destination, GetTileWidth());
}