#include <gameboy/ppu.hpp>

using namespace Amber;
using namespace Gameboy;

PPU::PPU()
{
	Reset();
}

uint8_t PPU::GetLY() const noexcept
{
	return static_cast<uint8_t>(m_Counter / LineCycles);
}

void PPU::Tick()
{
	++m_Counter;
	if (m_Counter == FrameCycles)
	{
		m_Counter = 0;
	}


}

void PPU::Reset()
{
	m_Counter = FrameCycles;
}