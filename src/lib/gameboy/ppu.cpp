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
	return m_VCounter;
}

void PPU::SetCPU(CPU* a_CPU) noexcept
{
	m_CPU = a_CPU;
}

void PPU::Tick()
{
	// Increment counters
	++m_HCounter;
	if (m_HCounter == LineCycles)
	{
		m_HCounter = 0;
		++m_VCounter;
		if (m_VCounter == FrameLines)
		{
			m_VCounter = 0;
		}
	}

	// Update LCD mode
	switch (m_LCDMode)
	{
		case LCDMode::HBlank:
		if (m_HCounter == 0)
		{
			if (m_VCounter == ScreenLines)
			{
				m_LCDMode = LCDMode::VBlank;
				if (m_CPU != nullptr)
				{
					m_CPU->RequestInterrupts(CPU::InterruptVBlank);
				}
			}
			else
			{
				m_LCDMode = LCDMode::OAMSearch;
			}
		}
		break;

		case LCDMode::VBlank:
		if (m_VCounter == 0)
		{
			m_LCDMode = LCDMode::OAMSearch;
		}
		break;

		case LCDMode::OAMSearch:
		if (m_HCounter == 20)
		{
			m_LCDMode = LCDMode::PixelTransfer;
		}
		break;

		case LCDMode::PixelTransfer:
		// TODO: accurate timing
		if (m_HCounter == 63)
		{
			m_LCDMode = LCDMode::HBlank;
		}
		break;
	}

	// Process LCD Mode
	switch (m_LCDMode)
	{
		case LCDMode::PixelTransfer:
		{
			// Just drawing some test data to see if the screen works
			const uint8_t x = (m_HCounter - 20) * 2;
			const uint8_t y = m_VCounter;
			SetPixel(x, y, (x + y) % 4);
			SetPixel(x + 1, y, (x + 1 +y) % 4);
		}
		break;
	}
}

void PPU::Reset()
{
	m_HCounter = LineCycles - 1;
	m_VCounter = FrameLines - 1;
	m_LCDMode = LCDMode::VBlank;
}

void PPU::Blit(void* a_Destination, size_t a_Pitch) const
{
	static constexpr uint8_t colors[] = { 0xFF, 0x77, 0xCC, 0x00 };

	for (size_t y = 0; y < LCDHeight; ++y)
	{
		for (size_t x = 0; x < LCDWidth; ++x)
		{
			const uint8_t color = colors[GetPixel(x, y)];

			uint8_t* const pixel = reinterpret_cast<uint8_t*>(a_Destination) + (y * a_Pitch + x) * 4;

			pixel[0] = color;
			pixel[1] = color;
			pixel[2] = color;
			pixel[3] = 0xFF;
		}
	}
}

uint8_t PPU::GetPixel(uint8_t a_X, uint8_t a_Y) const noexcept
{
	const size_t byte_offset = (static_cast<size_t>(a_X) + static_cast<size_t>(a_Y) * LCDWidth) / 4;
	const uint8_t bit_offset = (3 - (a_X % 4)) * 2;

	const uint8_t pixel_mask = (0b11 << bit_offset);

	return (m_LCDBuffer[byte_offset] & pixel_mask) >> bit_offset;
}

void PPU::SetPixel(uint8_t a_X, uint8_t a_Y, uint8_t a_Color) noexcept
{
	const size_t byte_offset = (static_cast<size_t>(a_X) + static_cast<size_t>(a_Y) * LCDWidth) / 4;
	const uint8_t bit_offset = (3 - (a_X % 4)) * 2;

	const uint8_t pixel_mask = ~(0b11 << bit_offset);

	m_LCDBuffer[byte_offset] = (m_LCDBuffer[byte_offset] & pixel_mask) | ((a_Color & 0b11) << bit_offset);
}