#include <gameboy/ppu.hpp>

#include <gameboy/cpu.hpp>
#include <gameboy/mmu.hpp>

using namespace Amber;
using namespace Gameboy;

PPU::PPU(MMU& a_MMU):
	m_MMU(a_MMU)
{
	Reset();
}

uint8_t PPU::GetLY() const noexcept
{
	return static_cast<uint8_t>(m_VCounter);
}

uint8_t PPU::GetLCDC() const noexcept
{
	return m_LCDC;
}

uint8_t PPU::GetSTAT() const noexcept
{
	return m_STAT;
}

uint8_t PPU::GetSCX() const noexcept
{
	return m_SCX;
}

uint8_t PPU::GetSCY() const noexcept
{
	return m_SCY;
}

void PPU::SetCPU(CPU* a_CPU) noexcept
{
	m_CPU = a_CPU;
}

void PPU::SetLCDC(uint8_t a_Value) noexcept
{
	m_LCDC = a_Value;
}

void PPU::SetSTAT(uint8_t a_Value) noexcept
{
	m_STAT = (m_STAT & 0b0000'0111) | (a_Value & 0b1111'1000);
}

void PPU::SetSCX(uint8_t a_Value) noexcept
{
	m_SCX = a_Value;
}

void PPU::SetSCY(uint8_t a_Value) noexcept
{
	m_SCY = a_Value;
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
				GotoOAM();
			}
		}
		break;

		case LCDMode::VBlank:
		if (m_VCounter == 0)
		{
			GotoOAM();
		}
		break;

		case LCDMode::OAMSearch:
		if (m_HCounter == OAMCycles)
		{
			m_LCDMode = LCDMode::PixelTransfer;
		}
		break;

		case LCDMode::PixelTransfer:
		// TODO: accurate timing
		if (m_HCounter == 240)
		{
			m_LCDMode = LCDMode::HBlank;
		}
		break;
	}

	// Update STAT
	m_STAT = (m_STAT & 0b1111'1000) | m_LCDMode;

	// Process LCD Mode
	switch (m_LCDMode)
	{
		case LCDMode::OAMSearch:
		OAMSearch();
		break;

		case LCDMode::PixelTransfer:
		PixelTransfer();
		break;
	}
}

void PPU::Reset()
{
	m_HCounter = LineCycles - 1;
	m_VCounter = FrameLines - 1;
	m_LCDMode = LCDMode::VBlank;
}

void PPU::Blit(void* a_Destination, size_t a_Pitch) const noexcept
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

void PPU::GotoOAM() noexcept
{
	m_LCDMode = LCDMode::OAMSearch;
	m_SpriteCount = 0;
}

void PPU::OAMSearch() noexcept
{
	const uint8_t sprite_index = m_HCounter / 2;

	// Load the first byte
	if ((m_HCounter & 1) == 0)
	{
		m_SpriteY = m_MMU.Load8(0xFE00 + sprite_index * 4);
		return;
	}
	
	// Load the second byte
	const uint8_t sprite_x = m_MMU.Load8(0xFE00 + sprite_index * 4 + 1);

	// No more than 10 sprites allowed
	if (m_SpriteCount == 10)
	{
		return;
	}

	// Check if it is on the current line
	if (m_SpriteY > m_VCounter + 16 || m_SpriteY + 8 <= m_VCounter + 16)
	{
		return;
	}

	// Check if it is on the screen horizontally
	if (sprite_x == 0 || sprite_x >= 168)
	{
		return;
	}

	// Add sprite to the list
	m_Sprites[m_SpriteCount] = sprite_index;
	++m_SpriteCount;
}

void PPU::PixelTransfer() noexcept
{
	// Calculate current screen position
	const uint8_t screen_x = static_cast<uint8_t>(m_HCounter - OAMCycles);
	const uint8_t screen_y = static_cast<uint8_t>(m_VCounter);

	// Check if it is covered by a sprite
	uint8_t sprite[4] = { 0, 0xFF, 0, 0 };
	for (uint8_t i = 0; i < m_SpriteCount; ++i)
	{
		const uint8_t sprite_index = m_Sprites[i];
		const uint8_t sprite_y = m_MMU.Load8(0xFE00 + sprite_index * 4 + 0);
		const uint8_t sprite_x = m_MMU.Load8(0xFE00 + sprite_index * 4 + 1);

		if (sprite_x > screen_x + 8 || sprite_x + 8 <= screen_x + 8)
		{
			continue;
		}

		if (sprite_y > screen_y + 16 || sprite_y + 8 <= screen_y + 16)
		{
			continue;
		}

		if (sprite_x >= sprite[1])
		{
			continue;
		}

		sprite[0] = sprite_y;
		sprite[1] = sprite_x;
		sprite[2] = m_MMU.Load8(0xFE00 + sprite_index * 4 + 2);
		sprite[3] = m_MMU.Load8(0xFE00 + sprite_index * 4 + 3);
	}

	const uint8_t scroll_x = screen_x + m_SCX;
	const uint8_t scroll_y = screen_y + m_SCY;

	uint8_t tile_index;
	uint8_t tile_x;
	uint8_t tile_y;
	uint16_t tile_data_address = 0x8000;

	if (sprite[0] != 0)
	{
		tile_index = sprite[2];
		tile_x = screen_x - (sprite[1] - 8);
		tile_y = screen_y - (sprite[0] - 16);
	}
	else
	{
		const uint8_t background_x = scroll_x / 8;
		const uint8_t background_y = scroll_y / 8;

		const uint16_t tile_index_offset = background_x + background_y * 32;
		tile_index = m_MMU.Load8(0x9800 + tile_index_offset);
		if ((m_LCDC & 0b0001'0000) == 0)
		{
			tile_index += 128;
			tile_data_address = 0x8800;
		}

		tile_x = scroll_x % 8;
		tile_y = scroll_y % 8;
	}

	uint8_t tile_data[16];
	for (uint16_t i = 0; i < 16; ++i)
	{
		tile_data[i] = m_MMU.Load8(tile_data_address + (tile_index * 16 + i));
	}

	const uint8_t line = tile_y * 2;

	const uint8_t byte0 = tile_data[line + 0];
	const uint8_t byte1 = tile_data[line + 1];

	const uint8_t bit0 = (byte0 >> (7 - tile_x)) & 0x01;
	const uint8_t bit1 = (byte1 >> (7 - tile_x)) & 0x01;

	const uint8_t color = bit0 | (bit1 << 1);

	SetPixel(screen_x, screen_y, color);
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