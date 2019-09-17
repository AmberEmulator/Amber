#ifndef H_AMBER_COMMON_CPUHELPER
#define H_AMBER_COMMON_CPUHELPER

#include <common/api.hpp>
#include <common/memory.hpp>
#include <common/register.hpp>

#include <limits>
#include <type_traits>

namespace Amber::Common
{
	namespace Internal
	{
		template <typename CPU, typename RegisterType, size_t RegisterCount, size_t ProgramCounter>
		class CPUHelperBase
		{
			public:
			using MicroOp = void (CPU::*)();

			CPUHelperBase(Memory<RegisterType>& a_Memory) noexcept:
				m_Memory(a_Memory)
			{
			}

			// Register load and store
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

			// Program counter
			RegisterType LoadProgramCounter() const noexcept
			{
				return LoadRegister<RegisterType>(ProgramCounter);
			}

			void StoreProgramCounter(RegisterType a_Value) noexcept
			{
				StoreRegister<RegisterType>(ProgramCounter, a_Value);
			}

			template <typename T>
			T PeekNext() const noexcept
			{
				const auto pc = LoadProgramCounter();
				return m_Memory.Load<T>(pc);
			}

			template <typename T>
			T ReadNext() noexcept
			{
				auto pc = LoadProgramCounter();
				const auto result = m_Memory.Load<T>(pc);
				pc += sizeof(T);
				StoreProgramCounter(pc);

				return result;
			}

			// Composition ops
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

			// Load ops
			template <typename T, size_t Destination, size_t Source>
			void LoadOp_r_r()
			{
				const T value = LoadRegister<T>(Source);
				StoreRegister<T>(Destination, value);
			}

			protected:
			Memory<RegisterType>& m_Memory;
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

		template <typename CPU, typename RegisterType, size_t RegisterCount, size_t ProgramCounter, bool Enabled = sizeof(RegisterType) >= 2>
		class CPUHelper16 : public CPUHelperBase<CPU, RegisterType, RegisterCount, ProgramCounter>
		{
			using CPUHelperBase::CPUHelperBase;
		};

		template <typename CPU, typename RegisterType, size_t RegisterCount, size_t ProgramCounter>
		class CPUHelper16<CPU, RegisterType, RegisterCount, ProgramCounter, true> : public CPUHelperBase<CPU, RegisterType, RegisterCount, ProgramCounter>
		{
			public:
			using CPUHelperBase::CPUHelperBase;

			// Register load and store
			uint16_t LoadRegister16(size_t a_Register) const noexcept
			{
				return LoadRegister<uint16_t>(a_Register);
			}

			void StoreRegister16(size_t a_Register, uint16_t a_Value) noexcept
			{
				StoreRegister<uint16_t>(a_Register, a_Value);
			}

			// Program counter
			uint16_t PeekNext16() const noexcept
			{
				return PeekNext<uint16_t>();
			}

			uint16_t ReadNext16() noexcept
			{
				return ReadNext<uint16_t>();
			}

			// Composition ops
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

			// Load ops
			template <size_t Destination, size_t Source>
			void LoadOp_r16_r16()
			{
				LoadOp_r_r<uint16_t, Destination, Source>();
			}
		};

		template <typename CPU, typename RegisterType, size_t RegisterCount, size_t ProgramCounter>
		class CPUHelper8 : public CPUHelper16<CPU, RegisterType, RegisterCount, ProgramCounter>
		{
			public:
			using CPUHelper16::CPUHelper16;

			// Register load and store
			uint8_t LoadRegister8(size_t a_Register) const noexcept
			{
				return LoadRegister<uint8_t>(a_Register);
			}

			void StoreRegister8(size_t a_Register, uint8_t a_Value) noexcept
			{
				StoreRegister<uint8_t>(a_Register, a_Value);
			}

			// Program counter
			uint8_t PeekNext8() const noexcept
			{
				return PeekNext<uint8_t>();
			}

			uint8_t ReadNext8() noexcept
			{
				return ReadNext<uint8_t>();
			}

			// Composition ops
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

			// Load ops
			template <size_t Destination, size_t Source>
			void LoadOp_r8_r8()
			{
				LoadOp_r_r<uint8_t, Destination, Source>();
			}
		};
	}

	template <typename CPU, typename RegisterType, size_t RegisterCount, size_t ProgramCounter>
	class CPUHelper : public Internal::CPUHelper8<CPU, RegisterType, RegisterCount, ProgramCounter>
	{
		public:
		using CPUHelper8::CPUHelper8;
	};
}

#endif