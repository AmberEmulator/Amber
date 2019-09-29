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
		bool IsFetchingSprite() const noexcept;
		const uint8_t* GetColors() const noexcept;

		void Tick();
		void Next();
		void Reset(uint16_t a_TileIndexAddress, uint8_t a_TileY, bool a_SignedIndex);
		void PushSpriteFetch(uint16_t a_TileIndexAddress, uint8_t a_TileY);
		void PopSpriteFetch();

		private:
		enum class State : uint8_t
		{
			ReadTile,
			ReadData0,
			ReadData1,
			Done,
		};

		struct FetchLocation
		{
			uint16_t m_TileIndexAddress;
			uint8_t m_TileY;
			bool m_SignedIndex;
		};

		FetchLocation& GetCurrentFetchLocation() noexcept;
		const FetchLocation& GetCurrentFetchLocation() const noexcept;

		Common::Memory16& m_Memory;
		FetchLocation m_FetchStack[2] = {};
		uint8_t m_FetchStackPointer = 0;

		State m_State;
		uint16_t m_TileAddress;
		uint8_t m_Colors[2];
	};
}

#endif