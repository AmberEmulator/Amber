#ifndef H_AMBER_GAMEBOY_TILEFETCHER
#define H_AMBER_GAMEBOY_TILEFETCHER

#include <gameboy/api.hpp>

#include <common/memory.hpp>

namespace Amber::Gameboy
{
	class GAMEBOY_API TileFetcher
	{
		public:
		TileFetcher(Common::Memory16& a_Memory);

		bool IsDone() const noexcept;
		const uint8_t* GetColors() const noexcept;

		void Tick();
		void Next();
		void FetchBackgroundTile(uint8_t a_X, uint8_t a_Y, uint8_t a_LCDC);
		void FetchSprite(uint8_t a_SpriteIndex, uint8_t a_TileY, uint8_t a_Attributes);

		private:
		enum class State : uint8_t
		{
			ReadTile,
			ReadData0,
			ReadData1,
			Done,
		};

		// Memory location
		Common::Memory16& m_Memory;
		uint8_t m_X = 0;
		uint8_t m_Y = 0;
		uint8_t m_TileY = 0;
		uint16_t m_TileIndexAddress;
		bool m_SignedIndex;

		// Intermediate state
		State m_State;
		uint16_t m_TileAddress;
		uint8_t m_Colors[2];
	};
}

#endif