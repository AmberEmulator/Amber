#include <gameboy/register.hpp>

using namespace Amber;
using namespace Gameboy;

uint8_t Register::Load8(uint8_t a_Index) const noexcept
{
	return m_Data.m_8[1 - a_Index];
}

uint16_t Register::Load16() const noexcept
{
	// TODO: endianness
	return m_Data.m_16;
}

void Register::Store8(uint8_t a_Index, uint8_t a_Value) noexcept
{
	m_Data.m_8[1 - a_Index] = a_Value;
}

void Register::Store16(uint16_t a_Value) noexcept
{
	// TODO: endianness
	m_Data.m_16 = a_Value;
}