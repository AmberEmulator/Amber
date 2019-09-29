#include <gameboy/tilefetcher.hpp>

using namespace Amber;
using namespace Common;
using namespace Gameboy;

TileFetcher::TileFetcher(Memory16& a_Memory):
	m_Memory(a_Memory)
{
}

bool TileFetcher::IsDone() const noexcept
{
	return m_State == State::Done;
}

bool TileFetcher::IsFetchingSprite() const noexcept
{
	return m_FetchStackPointer == 1;
}

const uint8_t* TileFetcher::GetColors() const noexcept
{
	return m_Colors;
}

void TileFetcher::Tick()
{
	switch (m_State)
	{
		case State::ReadTile:
		{
			auto& location = GetCurrentFetchLocation();

			const uint8_t tile_index = m_Memory.Load8(location.m_TileIndexAddress) + (location.m_SignedIndex ? 128 : 0);
			const uint16_t tile_base_address = location.m_SignedIndex ? 0x8800 : 0x8000;
			m_TileAddress = tile_base_address + tile_index * 16 + location.m_TileY * 2;

			m_State = State::ReadData0;
		}
		break;

		case State::ReadData0:
		m_Colors[0] = m_Memory.Load8(m_TileAddress);
		m_State = State::ReadData1;
		break;

		case State::ReadData1:
		m_Colors[1] = m_Memory.Load8(m_TileAddress + 1);
		m_State = State::Done;
		break;
	}
}

void TileFetcher::Next()
{
	auto& location = GetCurrentFetchLocation();
	++location.m_TileIndexAddress;
	m_State = State::ReadTile;
}

void TileFetcher::Reset(uint16_t a_TileIndexAddress, uint8_t a_TileY, bool a_SignedIndex)
{
	m_FetchStack[0].m_TileIndexAddress = a_TileIndexAddress;
	m_FetchStack[0].m_TileY = a_TileY;
	m_FetchStack[0].m_SignedIndex = a_SignedIndex;
	m_FetchStackPointer = 0;

	m_State = State::ReadTile;
}

void TileFetcher::PushSpriteFetch(uint16_t a_TileIndexAddress, uint8_t a_TileY)
{
	m_FetchStack[1].m_TileIndexAddress = a_TileIndexAddress;
	m_FetchStack[1].m_TileY = a_TileY;
	m_FetchStackPointer = 1;

	m_State = State::ReadTile;
}

void TileFetcher::PopSpriteFetch()
{
	m_FetchStackPointer = 0;
	m_State = State::ReadTile;
}

TileFetcher::FetchLocation& TileFetcher::GetCurrentFetchLocation() noexcept
{
	return const_cast<FetchLocation&>(static_cast<const TileFetcher*>(this)->GetCurrentFetchLocation());
}

const TileFetcher::FetchLocation& TileFetcher::GetCurrentFetchLocation() const noexcept
{
	return m_FetchStack[m_FetchStackPointer];
}