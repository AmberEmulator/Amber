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
			const uint8_t tile_index = m_Memory.Load8(m_TileIndexAddress) + (m_SignedIndex ? 128 : 0);
			const uint16_t tile_base_address = m_SignedIndex ? 0x8800 : 0x8000;
			m_TileAddress = tile_base_address + tile_index * 16 + m_TileY * 2;
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
	++m_TileIndexAddress;
	m_State = State::ReadTile;
}

void TileFetcher::Reset(uint16_t a_TileIndexAddress, uint8_t a_TileY, bool a_SignedIndex)
{
	m_TileIndexAddress = a_TileIndexAddress;
	m_TileY = a_TileY;
	m_SignedIndex = a_SignedIndex;
	m_State = State::ReadTile;
}