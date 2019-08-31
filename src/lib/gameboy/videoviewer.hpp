#ifndef H_GAMEBOY_COMMON_VIDEOVIEWER
#define H_GAMEBOY_COMMON_VIDEOVIEWER

#include <gameboy/api.hpp>

#include <common/ram.hpp>
#include <common/videoviewer.hpp>

namespace Amber::Gameboy
{
	class GAMEBOY_API VideoViewer : public Common::VideoViewer
	{
		public:
		VideoViewer(const Common::RAM16<false>& a_VRAM);
		~VideoViewer() noexcept override = default;

		size_t GetTileCount() const noexcept override;
		size_t GetTileWidth() const noexcept override;

		void BlitTile(size_t a_Tile, void* a_Destination, size_t a_Pitch) const override;

		private:
		const Common::RAM16<false>& m_VRAM;
	};
}

#endif