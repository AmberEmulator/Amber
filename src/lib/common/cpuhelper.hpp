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

			template <typename T> using UnaryOp = T (CPU::*)(T a_Value);
			template <typename T> using BinaryOp = T (CPU::*)(T a_Left, T a_Right);

			protected:
			Register<RegisterType> m_Registers[RegisterCount];
		};

		/*template <typename CPU, typename RegisterType, size_t RegisterCount>
		class CPUHelper64 : public CPUHelperBase<CPU, RegisterType, RegisterCount>
		{
			static constexpr size_t RegisterShift = (sizeof(RegisterType) / sizeof(uint64_t)) - 1;
			static constexpr size_t RegisterMask = 0b1111 >> (4 - RegisterShift);

			using UnaryOp64 = UnaryOp<uint64_t>;
			using BinaryOp64 = BinaryOp<uint64_t>;
		};

		template <typename CPU, typename RegisterType, size_t RegisterCount>
		class CPUHelper32 : public std::conditional_t<sizeof(RegisterType) == 4, CPUHelperBase<CPU, RegisterType, RegisterCount>, CPUHelper64<CPU, RegisterType, RegisterCount>>
		{
			static constexpr size_t RegisterShift = (sizeof(RegisterType) / sizeof(uint32_t)) - 1;
			static constexpr size_t RegisterMask = 0b1111 >> (4 - RegisterShift);

			using UnaryOp32 = UnaryOp<uint32_t>;
			using BinaryOp32 = BinaryOp<uint32_t>;
		};*/

		template <typename CPU, typename RegisterType, size_t RegisterCount, bool Enabled = sizeof(RegisterType) >= 2>
		class CPUHelper16 : public CPUHelperBase<CPU, RegisterType, RegisterCount>
		{
		};

		template <typename CPU, typename RegisterType, size_t RegisterCount>
		class CPUHelper16<CPU, RegisterType, RegisterCount, true> : public CPUHelperBase<CPU, RegisterType, RegisterCount>
		{
			public:
			static constexpr size_t RegisterShift = (sizeof(RegisterType) / sizeof(uint16_t)) - 1;
			static constexpr size_t RegisterMask = 0b1111 >> (4 - RegisterShift);

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

			using UnaryOp16 = CPUHelperBase::UnaryOp<uint16_t>;
			using BinaryOp16 = CPUHelperBase::BinaryOp<uint16_t>;

			template <uint8_t Destination, UnaryOp16 Op, bool Store = true>
			void UnaryOp_r16() noexcept
			{
				const uint16_t destination_value = LoadRegister16(Destination);
				const uint16_t result = (static_cast<CPU*>(this)->*Op)(destination_value);
				if constexpr (Store)
				{
					StoreRegister16(Destination, result);
				}
			}

			template <uint8_t Destination, uint8_t Source, BinaryOp16 Op, bool Store = true>
			void BinaryOp_r16_r16() noexcept
			{
				const uint16_t destination_value = LoadRegister16(Destination);
				const uint16_t source_value = LoadRegister16(Source);
				const uint16_t result = (static_cast<CPU*>(this)->*Op)(destination_value, source_value);
				if constexpr (Store)
				{
					StoreRegister16(Destination, result);
				}
			}
		};

		template <typename CPU, typename RegisterType, size_t RegisterCount>
		class CPUHelper8 : public CPUHelper16<CPU, RegisterType, RegisterCount>
		{
			static constexpr size_t RegisterShift = (sizeof(RegisterType) / sizeof(uint8_t)) - 1;
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

			using UnaryOp8 = CPUHelperBase::UnaryOp<uint8_t>;
			using BinaryOp8 = CPUHelperBase::BinaryOp<uint8_t>;

			template <uint8_t Destination, UnaryOp8 Op, bool Store = true>
			void UnaryOp_r8() noexcept
			{
				const uint8_t destination_value = LoadRegister8(Destination);
				const uint8_t result = (static_cast<CPU*>(this)->*Op)(destination_value);
				if constexpr (Store)
				{
					StoreRegister8(Destination, result);
				}
			}

			template <uint8_t Destination, uint8_t Source, BinaryOp8 Op, bool Store = true>
			void BinaryOp_r8_r8() noexcept
			{
				const uint8_t destination_value = LoadRegister8(Destination);
				const uint8_t source_value = LoadRegister8(Source);
				const uint8_t result = (static_cast<CPU*>(this)->*Op)(destination_value, source_value);
				if constexpr (Store)
				{
					StoreRegister8(Destination, result);
				}
			}
		};
	}

	template <typename CPU, typename RegisterType, size_t RegisterCount>
	class CPUHelper : public Internal::CPUHelper8<CPU, RegisterType, RegisterCount>
	{
	};

}

#endif