#ifndef H_AMBER_GAMEBOY_JOYPAD
#define H_AMBER_GAMEBOY_JOYPAD

#include <gameboy/api.hpp>

namespace Amber::Gameboy
{
	class CPU;

	class GAMEBOY_API Joypad
	{
		public:
		static constexpr uint8_t DirectionSelectBit = 4;
		static constexpr uint8_t ButtonSelectBit = 5;
		static constexpr uint8_t DirectionSelectMask = 1 << DirectionSelectBit;
		static constexpr uint8_t ButtonSelectMask = 1 << ButtonSelectBit;

		void SetCPU(CPU* a_CPU) noexcept;
		void SetButtonState(uint8_t a_Buttons, bool a_State) noexcept;
		void SetRegister(uint8_t a_Value) noexcept;

		CPU* GetCPU() const noexcept;
		uint8_t GetButtonState(uint8_t a_Buttons) const noexcept;
		uint8_t GetRegister() const noexcept;

		private:
		void UpdateRegister();

		CPU* m_CPU = nullptr;
		uint8_t m_ButtonStates = 0b11111111;
		uint8_t m_Register = 0b11001111;
	};
}

#endif