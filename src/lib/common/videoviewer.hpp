#ifndef H_AMBER_COMMON_VIDEOVIEWER
#define H_AMBER_COMMON_VIDEOVIEWER

#include <common/api.hpp>

namespace Amber::Common
{
	class COMMON_API VideoViewer
	{
		public:
		virtual ~VideoViewer() noexcept = 0;

		virtual size_t GetTileCount() const noexcept = 0;
		virtual size_t GetTileWidth() const noexcept = 0;
		virtual size_t GetTileHeight() const noexcept;

		void BlitTile(size_t a_Tile, void* a_Destination) const;
		virtual void BlitTile(size_t a_Tile, void* a_Destination, size_t a_Pitch) const = 0;
	};
}

#endif