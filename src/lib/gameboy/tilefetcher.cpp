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
	m_X += 8;

	const uint8_t background_x = m_X / 8;
	const uint8_t background_y = m_Y / 8;
	m_TileIndexAddress = 0x9800 + background_x + background_y * 32;

	m_State = State::ReadTile;
}

void TileFetcher::FetchBackgroundTile(uint8_t a_X, uint8_t a_Y, uint8_t a_LCDC)
{
	m_X = a_X - 8;
	m_Y = a_Y;
	m_TileY = m_Y % 8;
	m_SignedIndex = (a_LCDC & 0b0001'0000) == 0;

	Next();
}

void TileFetcher::FetchSprite(uint8_t a_SpriteIndex, uint8_t a_TileY, uint8_t a_Attributes)
{
	m_TileIndexAddress = 0xFE00 + a_SpriteIndex * 4 + 2;
	m_TileY = a_TileY;
	m_SignedIndex = false;

	m_State = State::ReadTile;
}