#include <gameboy/joypad.hpp>

#include <gameboy/cpu.hpp>

using namespace Amber;
using namespace Gameboy;

void Joypad::SetCPU(CPU* a_CPU) noexcept
{
	m_CPU = a_CPU;
}

void Joypad::SetButtonState(uint8_t a_Buttons, bool a_State) noexcept
{
	if (a_State)
	{
		m_ButtonStates &= ~a_Buttons;
	}
	else
	{
		m_ButtonStates |= a_Buttons;
	}

	UpdateRegister();
}

void Joypad::SetRegister(uint8_t a_Value) noexcept
{
	m_Register = (a_Value & 0b11110000) | (m_Register & 0b00001111);
	UpdateRegister();
}

CPU* Joypad::GetCPU() const noexcept
{
	return m_CPU;
}

uint8_t Joypad::GetButtonState(uint8_t a_Buttons) const noexcept
{
	return (~m_ButtonStates) & a_Buttons;
}

uint8_t Joypad::GetRegister() const noexcept
{
	return m_Register;
}

void Joypad::UpdateRegister()
{
	// Calculate new button state
	uint8_t button_states = 0b1111;

	// Set direction states if selected
	if ((~m_Register) & DirectionSelectMask)
	{
		button_states &= m_ButtonStates & 0b1111;
	}

	// Set button states if selected
	if ((~m_Register) & ButtonSelectMask)
	{
		button_states &= m_ButtonStates >> 4;
	}

	// Check if a button was pressed
	if (m_CPU != nullptr)
	{
		if ((m_Register & 0b11110000) & (~button_states))
		{
			m_CPU->RequestInterrupts(CPU::InterruptJoypad);
		}
	}

	// Update register
	m_Register &= 0b11110000;
	m_Register |= button_states;
}