#ifndef H_AMBER_COMMON_CPUHELPER
#define H_AMBER_COMMON_CPUHELPER

#include <common/api.hpp>
#include <common/register.hpp>

#include <limits>
#include <type_traits>

namespace Amber::Common
{
	namespace Internal
	{
		template <typename CPU, typename RegisterType, size_t RegisterCount>
		class CPUHelperBase
		{
			public:
			using MicroOp = void (CPU::*)();

			protected:
			Register<RegisterType> m_Registers[RegisterCount];
		};

		template <typename CPU, typename RegisterType, size_t RegisterCount>
		class CPUHelper16 : public CPUHelperBase<CPU, RegisterType, RegisterCount>
		{
			public:
			static constexpr size_t RegisterShift = sizeof(RegisterType) >> 2;
			static constexpr size_t RegisterMask = 0b1111 >> (4 - RegisterShift);

			using UnaryOp16 = uint16_t(CPU::*)(uint16_t a_Value);
			using BinaryOp16 = uint16_t(CPU::*)(uint16_t a_Left, uint16_t a_Right);

			uint16_t LoadRegister16(uint8_t a_Register) const noexcept
			{
				auto& reg = m_Registers[a_Register >> RegisterShift];
				return reg.Load16(a_Register & RegisterMask);
			}

			void StoreRegister16(uint8_t a_Register, uint16_t a_Value) noexcept
			{
				auto& reg = m_Registers[a_Register >> RegisterShift];
				reg.Store16(a_Register & RegisterMask, a_Value);
			}
		};

		template <typename CPU, typename RegisterType, size_t RegisterCount>
		class CPUHelper8 : public std::conditional_t<sizeof(RegisterType) == 1, CPUHelperBase<CPU, RegisterType, RegisterCount>, CPUHelper16<CPU, RegisterType, RegisterCount>>
		{
			static constexpr size_t RegisterShift = sizeof(RegisterType) >> 1;
			static constexpr size_t RegisterMask = 0b1111 >> (4 - RegisterShift);

			public:
			uint8_t LoadRegister8(uint8_t a_Register) const noexcept
			{
				auto& reg = m_Registers[a_Register >> RegisterShift];
				return reg.Load8(a_Register & RegisterMask);
			}

			void StoreRegister8(uint8_t a_Register, uint8_t a_Value) noexcept
			{
				auto& reg = m_Registers[a_Register >> RegisterShift];
				reg.Store8(a_Register & RegisterMask, a_Value);
			}

			using UnaryOp8 = uint8_t(CPU::*)(uint8_t  a_Value);
			using BinaryOp8 = uint8_t(CPU::*)(uint8_t  a_Left, uint8_t  a_Right);
		};
	}

	template <typename CPU, typename RegisterType, size_t RegisterCount>
	class CPUHelper : public Internal::CPUHelper8<CPU, RegisterType, RegisterCount>
	{
	};

}

#endif