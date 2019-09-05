#ifndef H_AMBER_GAMEBOY_REGISTER
#define H_AMBER_GAMEBOY_REGISTER

#include <gameboy/api.hpp>

namespace Amber::Gameboy
{
	class Register
	{
		public:
		uint8_t Load8(uint8_t a_Index) const noexcept;
		uint16_t Load16() const noexcept;

		void Store8(uint8_t a_Index, uint8_t a_Value) noexcept;
		void Store16(uint16_t a_Value) noexcept;

		private:
		union
		{
			uint8_t m_8[2];
			uint16_t m_16 = 0;
		} m_Data;
	};
}

#endif