#include <gameboy/ppu.hpp>

#include <gameboy/cpu.hpp>
#include <gameboy/mmu.hpp>
#include <gameboy/ppuobserver.hpp>

#include <array>

using namespace Amber;
using namespace Gameboy;

PPU::PPU(MMU& a_MMU):
	m_MMU(a_MMU),
	m_TileFetcher(a_MMU)
{
	Reset();
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

uint8_t PPU::GetLY() const noexcept
{
	return static_cast<uint8_t>(m_VCounter);
}

uint8_t PPU::GetLYC() const noexcept
{
	return m_LYC;
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

LCDMode::Enum PPU::GetLCDMode() const noexcept
{
	return static_cast<LCDMode::Enum>(GetSTAT() & 0b11);
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

void PPU::SetLYC(uint8_t a_Value) noexcept
{
	m_LYC = (m_LYC & ReadOnlySTATMask) | (a_Value & WritableSTATMask);
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
	switch (GetLCDMode())
	{
		case LCDMode::HBlank:
		if (m_HCounter == 0)
		{
			if (m_VCounter == ScreenLines)
			{
				GotoVBlank();
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
		if (m_DrawX == LCDWidth + 7 + m_SCX % 8)
		{
			GotoHBlank();
		}
		break;
	}

	// Process LCD Mode
	switch (GetLCDMode())
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
	SetLCDMode(LCDMode::VBlank);

	// LCD control
	m_LCDC = 0x91;
	m_STAT = 0x00;
	m_SCX = 0x00;
	m_SCY = 0x00;
	m_LYC = 0x00;
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

void PPU::AddObserver(PPUObserver& a_Observer)
{
	m_Observers.insert(&a_Observer);
}

void PPU::RemoveObserver(PPUObserver& a_Observer)
{
	m_Observers.erase(&a_Observer);
}

void PPU::SetLCDMode(LCDMode::Enum a_Mode)
{
	const auto from_mode = GetLCDMode();
	if (from_mode == a_Mode)
	{
		return;
	}

	m_STAT = (m_STAT & (~ModeSTATMask)) | a_Mode;

	if (m_CPU != nullptr)
	{
		switch (a_Mode)
		{
			case LCDMode::HBlank:
			if (m_STAT & HBlankInterruptSTATMask)
			{
				m_CPU->RequestInterrupts(CPU::InterruptLCDSTAT);
			}
			break;

			case LCDMode::VBlank:
			if (m_STAT & VBlankInterruptSTATMask)
			{
				m_CPU->RequestInterrupts(CPU::InterruptLCDSTAT);
			}
			m_CPU->RequestInterrupts(CPU::InterruptVBlank);
			break;

			case LCDMode::OAMSearch:
			if (m_STAT & OAMInterruptSTATMask)
			{
				m_CPU->RequestInterrupts(CPU::InterruptLCDSTAT);
			}
			break;
		}
	}

	const auto to_mode = GetLCDMode();
	if (from_mode == LCDMode::VBlank && to_mode == LCDMode::OAMSearch)
	{
		std::memset(m_LCDBuffer, 0, sizeof(m_LCDBuffer));
	}

	for (auto& observer : m_Observers)
	{
		observer->OnLCDModeChange(from_mode, to_mode);
	}
}

void PPU::GotoOAM() noexcept
{
	const bool lyc_result = GetLY() == GetLYC();
	m_STAT = (m_STAT & (~LYCSTATMask)) | (lyc_result ? LYCSTATMask : 0);

	if (lyc_result && (m_STAT & LYCInterruptSTATMask) && m_CPU != nullptr)
	{
		m_CPU->RequestInterrupts(CPU::InterruptLCDSTAT);
	}

	m_SpriteCount = 0;

	SetLCDMode(LCDMode::OAMSearch);
}

void PPU::GotoPixelTransfer() noexcept
{
	m_CurrentSprite = 0;

	const uint8_t screen_y = static_cast<uint8_t>(m_VCounter);
	const uint8_t scroll_x = m_SCX;
	const uint8_t scroll_y = screen_y + m_SCY;
	const uint8_t background_x = scroll_x / 8;
	const uint8_t background_y = scroll_y / 8;

	const uint16_t background_address = 0x9800 + background_x + background_y * 32;
	const uint8_t tile_y = scroll_y % 8;

	m_PixelFIFO.Reset();
	m_TileFetcher.Reset(background_address, tile_y, (m_LCDC & 0b0001'0000) == 0);
	m_DrawX = 0;

	SetLCDMode(LCDMode::PixelTransfer);
}

void PPU::GotoHBlank() noexcept
{
	SetLCDMode(LCDMode::HBlank);
}

void PPU::GotoVBlank() noexcept
{
	SetLCDMode(LCDMode::VBlank);
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
		if (m_Sprites[index - 1].m_DrawX < sprite_x)
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
	sprite.m_SpriteIndex = sprite_index;
	sprite.m_TileY = extended_screen_y - m_SpriteY;
	sprite.m_DrawX = sprite_x - 1;
	sprite.m_Attributes = m_OAM[sprite_index * 4 + 3];

	if (sprite.m_Attributes & 0b0100'0000)
	{
		sprite.m_TileY = 7 - sprite.m_TileY;
	}

	// Increment sprite count
	++m_SpriteCount;
}

void PPU::PixelTransfer() noexcept
{
	if (m_CurrentSprite < m_SpriteCount && m_Sprites[m_CurrentSprite].m_DrawX == m_DrawX && !m_TileFetcher.IsFetchingSprite())
	{
		m_TileFetcher.PushSpriteFetch(0xFE00 + m_Sprites[m_CurrentSprite].m_SpriteIndex * 4 + 2, m_Sprites[m_CurrentSprite].m_TileY);
		m_SpriteAttributes = m_Sprites[m_CurrentSprite].m_Attributes;
		++m_CurrentSprite;
	}

	if ((m_HCounter & 1) == 0)
	{
		m_TileFetcher.Tick();

		if (m_TileFetcher.IsDone())
		{
			if (m_TileFetcher.IsFetchingSprite())
			{
				m_PixelFIFO.MixSprite(m_TileFetcher.GetColors(), m_SpriteAttributes);
				m_TileFetcher.PopSpriteFetch();
			}
			else if (m_PixelFIFO.GetPixelCount() <= 8)
			{
				m_PixelFIFO.Push(m_TileFetcher.GetColors(), PixelSource::Background);
				m_TileFetcher.Next();
			}
		}
	}

	if (m_TileFetcher.IsFetchingSprite())
	{
		return;
	}

	const Pixel pixel = m_PixelFIFO.Pop();
	if (pixel.IsNull())
	{
		return;
	}

	const uint8_t draw_offset = 7 + m_SCX % 8;
	if (m_DrawX < draw_offset)
	{
		++m_DrawX;
		return;
	}

	uint8_t palette;

	switch (pixel.GetSource())
	{
		case PixelSource::Background:
		case PixelSource::Window:
		palette = m_BGP;
		break;

		case PixelSource::Sprite0:
		palette = m_OBP0;
		break;
		
		case PixelSource::Sprite1:
		palette = m_OBP1;
		break;
	}

	const uint8_t color = (palette >> (pixel.GetColor() * 2)) & 0b11;

	const uint8_t screen_x = m_DrawX - draw_offset;
	const uint8_t screen_y = static_cast<uint8_t>(m_VCounter);
	SetPixel(screen_x, screen_y, color);
	++m_DrawX;
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