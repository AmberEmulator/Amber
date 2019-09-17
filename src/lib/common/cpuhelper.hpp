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

			template <typename T>
			T LoadRegister(size_t a_Register) const noexcept
			{
				static constexpr size_t RegisterShift = (sizeof(RegisterType) / sizeof(T)) - 1;
				static constexpr size_t RegisterMask = 0b1111 >> (4 - RegisterShift);

				auto& reg = m_Registers[a_Register >> RegisterShift];
				return reg.Load<T>(a_Register & RegisterMask);
			}

			template <typename T>
			void StoreRegister(size_t a_Register, T a_Value) noexcept
			{
				static constexpr size_t RegisterShift = (sizeof(RegisterType) / sizeof(T)) - 1;
				static constexpr size_t RegisterMask = 0b1111 >> (4 - RegisterShift);

				auto& reg = m_Registers[a_Register >> RegisterShift];
				reg.Store<T>(a_Register & RegisterMask, a_Value);
			}

			template <typename T> using UnaryOp = T (CPU::*)(T a_Value);
			template <typename T> using BinaryOp = T (CPU::*)(T a_Left, T a_Right);

			template <typename T, uint8_t Destination, UnaryOp<T> Op, bool Store = true>
			void UnaryOp_r() noexcept
			{
				const T destination_value = LoadRegister<T>(Destination);
				const T result = (static_cast<CPU*>(this)->*Op)(destination_value);
				if constexpr (Store)
				{
					StoreRegister<T>(Destination, result);
				}
			}

			template <typename T, uint8_t Destination, uint8_t Source, BinaryOp<T> Op, bool Store = true>
			void BinaryOp_r_r() noexcept
			{
				const T destination_value = LoadRegister<T>(Destination);
				const T source_value = LoadRegister<T>(Source);
				const T result = (static_cast<CPU*>(this)->*Op)(destination_value, source_value);
				if constexpr (Store)
				{
					StoreRegister<T>(Destination, result);
				}
			}

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
			uint16_t LoadRegister16(size_t a_Register) const noexcept
			{
				return LoadRegister<uint16_t>(a_Register);
			}

			void StoreRegister16(size_t a_Register, uint16_t a_Value) noexcept
			{
				StoreRegister<uint16_t>(a_Register, a_Value);
			}

			using UnaryOp16 = CPUHelperBase::UnaryOp<uint16_t>;
			using BinaryOp16 = CPUHelperBase::BinaryOp<uint16_t>;

			template <uint8_t Destination, UnaryOp16 Op, bool Store = true>
			void UnaryOp_r16() noexcept
			{
				UnaryOp_r<uint16_t, Destination, Op, Store>();
			}

			template <uint8_t Destination, uint8_t Source, BinaryOp16 Op, bool Store = true>
			void BinaryOp_r16_r16() noexcept
			{
				BinaryOp_r_r<uint16_t, Destination, Source, Op, Store>();
			}
		};

		template <typename CPU, typename RegisterType, size_t RegisterCount>
		class CPUHelper8 : public CPUHelper16<CPU, RegisterType, RegisterCount>
		{
			public:
			uint8_t LoadRegister8(size_t a_Register) const noexcept
			{
				return LoadRegister<uint8_t>(a_Register);
			}

			void StoreRegister8(size_t a_Register, uint8_t a_Value) noexcept
			{
				StoreRegister<uint8_t>(a_Register, a_Value);
			}

			using UnaryOp8 = CPUHelperBase::UnaryOp<uint8_t>;
			using BinaryOp8 = CPUHelperBase::BinaryOp<uint8_t>;

			template <uint8_t Destination, UnaryOp8 Op, bool Store = true>
			void UnaryOp_r8() noexcept
			{
				UnaryOp_r<uint8_t, Destination, Op, Store>();
			}

			template <uint8_t Destination, uint8_t Source, BinaryOp8 Op, bool Store = true>
			void BinaryOp_r8_r8() noexcept
			{
				BinaryOp_r_r<uint8_t, Destination, Source, Op, Store>();
			}
		};
	}

	template <typename CPU, typename RegisterType, size_t RegisterCount>
	class CPUHelper : public Internal::CPUHelper8<CPU, RegisterType, RegisterCount>
	{
	};

}

#endif