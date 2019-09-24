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

uint8_t PPU::GetBGP() const noexcept
{
	return m_BGP;
}

uint8_t PPU::GetOBP0() const noexcept
{
	return m_OBP0;
}

uint8_t PPU::GetOBP1() const noexcept
{
	return m_OBP1;
}

uint8_t* PPU::GetOAM() noexcept
{
	return const_cast<uint8_t*>(static_cast<const PPU*>(this)->GetOAM());
}

const uint8_t* PPU::GetOAM() const noexcept
{
	return m_OAM;
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

void PPU::SetBGP(uint8_t a_Value) noexcept
{
	m_BGP = a_Value;
}

void PPU::SetOBP0(uint8_t a_Value) noexcept
{
	m_OBP0 = a_Value;
}

void PPU::SetOBP1(uint8_t a_Value) noexcept
{
	m_OBP1 = a_Value;
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
			GotoPixelTransfer();
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
	// OAM
	std::memset(m_OAM, 0, sizeof(m_OAM));

	// LCD mode
	m_HCounter = LineCycles - 1;
	m_VCounter = FrameLines - 1;
	m_LCDMode = LCDMode::VBlank;

	// LCD control
	m_LCDC = 0x91;
	m_STAT = 0x00;
	m_SCX = 0x00;
	m_SCY = 0x00;
}

void PPU::Blit(void* a_Destination, size_t a_Pitch) const noexcept
{
	static constexpr uint8_t colors[] = { 0xFF, 0xCC, 0x77, 0x00 };

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

void PPU::GotoPixelTransfer() noexcept
{
	m_LCDMode = LCDMode::PixelTransfer;
	m_CurrentSprite = 0xFF;
}

void PPU::OAMSearch() noexcept
{
	const uint8_t sprite_index = m_HCounter / 2;

	// Load the first byte
	if ((m_HCounter & 1) == 0)
	{
		m_SpriteY = m_OAM[sprite_index * 4];
		return;
	}
	
	// Load the second byte
	const uint8_t sprite_x = m_OAM[sprite_index * 4 + 1];

	// No more than 10 sprites allowed
	if (m_SpriteCount == 10)
	{
		return;
	}

	// Check if it is on the current line
	const uint8_t extended_screen_y = static_cast<uint8_t>(m_VCounter + 16);
	if (m_SpriteY > extended_screen_y || m_SpriteY + 8 <= extended_screen_y)
	{
		return;
	}

	// Check if it is on the screen horizontally
	if (sprite_x == 0 || sprite_x >= 168)
	{
		return;
	}

	// Find insertion index
	uint8_t index = m_SpriteCount;
	for (; index != 0; --index)
	{
		if (m_Sprites[index - 1].m_ScreenX < sprite_x)
		{
			break;
		}
	}

	// Move sprites after insertion up
	for (uint8_t i = m_SpriteCount; i > index; --i)
	{
		m_Sprites[i] = m_Sprites[i - 1];
	}

	// Set sprite info at index
	auto& sprite = m_Sprites[index];
	sprite.m_TileY = extended_screen_y - m_SpriteY;
	sprite.m_ScreenX = sprite_x;
	sprite.m_Tile = m_OAM[sprite_index * 4 + 2];
	sprite.m_Attributes = m_OAM[sprite_index * 4 + 3];

	// Increment sprite count
	++m_SpriteCount;
}

void PPU::PixelTransfer() noexcept
{
	// Calculate current screen position
	const uint8_t screen_x = static_cast<uint8_t>(m_HCounter - OAMCycles);
	const uint8_t screen_y = static_cast<uint8_t>(m_VCounter);
	const uint8_t extended_screen_x = screen_x + 8;

	// Check if it is covered by a sprite
	for (uint8_t next_sprite = m_CurrentSprite + 1; next_sprite < m_SpriteCount; ++m_CurrentSprite, ++next_sprite)
	{
		const uint8_t next_sprite_x = m_Sprites[next_sprite].m_ScreenX;
		if (next_sprite_x > extended_screen_x)
		{
			break;
		}
	}

	SpriteDrawInfo sprite = {};
	sprite.m_ScreenX = 0xFF;
	if (m_CurrentSprite != 0xFF && m_Sprites[m_CurrentSprite].m_ScreenX + 8 > extended_screen_x)
	{
		sprite = m_Sprites[m_CurrentSprite];
	}

	const uint8_t scroll_x = screen_x + m_SCX;
	const uint8_t scroll_y = screen_y + m_SCY;

	uint8_t tile_index;
	uint8_t tile_x;
	uint8_t tile_y;
	uint16_t tile_data_address = 0x8000;
	uint8_t palette = 0;

	if (sprite.m_ScreenX != 0xFF)
	{
		tile_index = sprite.m_Tile;
		tile_x = extended_screen_x - sprite.m_ScreenX;
		tile_y = sprite.m_TileY;

		if (sprite.m_Attributes & 0b0001'0000)
		{
			palette = m_OBP1;
		}
		else
		{
			palette = m_OBP0;
		}
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

		palette = m_BGP;
	}

	const uint8_t line = tile_y * 2;

	const uint8_t byte0 = m_MMU.Load8(tile_data_address + (tile_index * 16 + line + 0));
	const uint8_t byte1 = m_MMU.Load8(tile_data_address + (tile_index * 16 + line + 1));

	const uint8_t bit0 = (byte0 >> (7 - tile_x)) & 0x01;
	const uint8_t bit1 = (byte1 >> (7 - tile_x)) & 0x01;

	const uint8_t color_index = bit0 | (bit1 << 1);
	const uint8_t color = (palette >> (color_index * 2)) & 0b11;

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