#include <gameboy/ppu.hpp>

#include <gameboy/cpu.hpp>

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

void PPU::SetCPU(CPU* a_CPU) noexcept
{
	m_CPU = a_CPU;
}

void PPU::Tick()
{
	++m_Counter;
	if (m_Counter == FrameCycles)
	{
		m_Counter = 0;
	}

	if (m_Counter == ScreenCycles && m_CPU != nullptr)
	{
		m_CPU->SetInterruptRequests(m_CPU->GetInterruptRequests() | CPU::InterruptVBlank);
	}
}


void PPU::Reset()
{
	m_Counter = FrameCycles - 1;
}