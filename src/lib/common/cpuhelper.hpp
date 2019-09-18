#ifndef H_AMBER_COMMON_CPUHELPER
#define H_AMBER_COMMON_CPUHELPER

#include <common/api.hpp>
#include <common/memory.hpp>
#include <common/register.hpp>

#include <cassert>
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

			protected:
			// Program counter
			RegisterType LoadProgramCounter() const noexcept
			{
				return LoadRegister<RegisterType>(ProgramCounter);
			}

			void StoreProgramCounter(RegisterType a_Value) noexcept
			{
				StoreRegister<RegisterType>(ProgramCounter, a_Value);
			}

			// Decoding
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

			template <auto Op, auto... Ops>
			void ConcatOp() noexcept
			{
				(static_cast<CPU*>(this)->*Op)();
				if constexpr (sizeof...(Ops) != 0)
				{
					ConcatOp<Ops...>();
				}
			}

			template <auto Op, uint8_t Counter>
			void DelayOp() noexcept
			{
				if constexpr (Counter == 0)
				{
					ExtendInstruction(Op);
				}
				else
				{
					PushOp(&CPU::DelayOp<Op, Counter - 1>);
				}
			}

			void SkipOp() noexcept
			{
				while (!IsInstructionDone())
				{
					PopOp();
				}
			}

			// Load ops
			template <typename T, size_t Destination>
			void LoadOp_r_x(T a_Value) noexcept
			{
				StoreRegister<T>(Destination, a_Value);
			}

			template <typename T, size_t Destination, size_t Source>
			void LoadOp_r_r() noexcept
			{
				const T value = LoadRegister<T>(Source);
				LoadOp_r_x<T, Destination>(value);
			}

			template <typename T, size_t Destination>
			void LoadOp_r_n() noexcept
			{
				const T value = ReadNext<T>();
				LoadOp_r_x<T, Destination>(value);
			}

			template <typename T, size_t Destination, size_t Source>
			void LoadOp_r_ar() noexcept
			{
				const RegisterType address = LoadRegister<RegisterType>(Source);
				const T value = m_Memory.Load<T>(address);
				LoadOp_r_x<T, Destination>(value);
			}

			template <typename T, size_t Destination, size_t Source>
			void LoadOp_ar_r() noexcept
			{
				const RegisterType address = LoadRegister<RegisterType>(Destination);
				const T value = LoadRegister<T>(Source);
				m_Memory.Store<T>(address, value);
			}

			// Jump ops
			void JumpOp_x(RegisterType a_Address) noexcept
			{
				StoreProgramCounter(a_Address);
			}

			template <size_t Source>
			void JumpOp_r() noexcept
			{
				const RegisterType address = LoadRegister<RegisterType>(Source);
				JumpOp_x(address);
			}

			template <RegisterType Address>
			void JumpOp() noexcept
			{
				JumpOp_x(Address);
			}

			// Managing op queue
			void PushOp(MicroOp a_Op) noexcept
			{
				#ifdef _DEBUG
				assert(m_MicroOps[m_OpBack] == nullptr);
				#endif

				m_MicroOps[m_OpBack] = a_Op;
				IncrementOpCounter(m_OpBack);
			}

			MicroOp PopOp() noexcept
			{
				const MicroOp op = m_MicroOps[m_OpFront];

				#ifdef _DEBUG
				assert(m_MicroOps[m_OpFront] != nullptr);
				m_MicroOps[m_OpFront] = nullptr;
				#endif

				IncrementOpCounter(m_OpFront);

				return op;
			}

			void CallOp(MicroOp a_Op)
			{
				(static_cast<CPU*>(this)->*a_Op)();
			}

			void ClearOps() noexcept
			{
				m_OpBack = m_OpFront;

				#ifdef _DEBUG
				std::memset(m_MicroOps, 0, sizeof(m_MicroOps));
				#endif
			}

			void PushInstruction(MicroOp a_Op) noexcept
			{
				PushInstruction(&a_Op, 1);
			}

			// TODO: use span
			void PushInstruction(const MicroOp* a_Ops, size_t a_Count) noexcept
			{
				for (size_t i = 0; i < a_Count; ++i)
				{
					PushOp(a_Ops[i]);
				}

				m_OpDone = m_OpBack;
			}

			void InsertOpBeforeInstruction(MicroOp a_Op)
			{
				InsertOp(a_Op, m_OpFront);
			}

			void InsertOpAfterInstruction(MicroOp a_Op)
			{
				InsertOp(a_Op, m_OpDone);
			}

			void ExtendInstruction(MicroOp a_Op) noexcept
			{
				ExtendInstruction(&a_Op, 1);
			}

			// TODO: use span
			void ExtendInstruction(const MicroOp* a_Ops, size_t a_Count) noexcept
			{
				for (size_t i = 0; i < a_Count; ++i)
				{
					InsertOpAfterInstruction(a_Ops[i]);
					IncrementInstruction();
				}
			}

			bool IsInstructionDone() const noexcept
			{
				return m_OpFront == m_OpDone;
			}

			void IncrementInstruction() noexcept
			{
				IncrementOpCounter(m_OpDone);
			}

			void ResetInstruction() noexcept
			{
				m_OpDone = m_OpFront;
			}

			// Memory variables
			Memory<RegisterType>& m_Memory;
			Register<RegisterType> m_Registers[RegisterCount];

			private:
			// Managing op queue
			void IncrementOpCounter(size_t& a_Counter) const noexcept
			{
				++a_Counter;
				a_Counter %= std::size(m_MicroOps);
			}

			void InsertOp(MicroOp a_Op, size_t a_Index) noexcept
			{
				for (size_t i = m_OpBack; i != a_Index; i = (i - 1) % std::size(m_MicroOps))
				{
					const size_t prev = (i - 1) % std::size(m_MicroOps);

					m_MicroOps[i] = m_MicroOps[prev];
				}

				m_MicroOps[a_Index] = a_Op;
				IncrementOpCounter(m_OpBack);
			}

			// Op queue variables
			MicroOp m_MicroOps[16] = {};
			size_t m_OpFront = 0;
			size_t m_OpBack = 0;
			size_t m_OpDone = 0;
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
			template <size_t Destination>
			void LoadOp_r16_x16(uint16_t a_Value) noexcept
			{
				LoadOp_r_x<uint16_t, Destination>(a_Value);
			}

			template <size_t Destination, size_t Source>
			void LoadOp_r16_r16() noexcept
			{
				LoadOp_r_r<uint16_t, Destination, Source>();
			}

			template <size_t Destination>
			void LoadOp_r16_n16() noexcept
			{
				LoadOp_r_n<uint16_t, Destination>();
			}

			template <size_t Destination, size_t Source>
			void LoadOp_r16_ar() noexcept
			{
				LoadOp_r_ar<uint16_t, Destination, Source>();
			}

			template <size_t Destination, size_t Source>
			void LoadOp_ar_r16() noexcept
			{
				LoadOp_ar_r<uint16_t, Destination, Source>();
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
			template <size_t Destination>
			void LoadOp_r8_x8(uint8_t a_Value) noexcept
			{
				LoadOp_r_x<uint8_t, Destination>(a_Value);
			}

			template <size_t Destination, size_t Source>
			void LoadOp_r8_r8() noexcept
			{
				LoadOp_r_r<uint8_t, Destination, Source>();
			}

			template <size_t Destination>
			void LoadOp_r8_n8() noexcept
			{
				LoadOp_r_n<uint8_t, Destination>();
			}

			template <size_t Destination, size_t Source>
			void LoadOp_r8_ar() noexcept
			{
				LoadOp_r_ar<uint8_t, Destination, Source>();
			}

			template <size_t Destination, size_t Source>
			void LoadOp_ar_r8() noexcept
			{
				LoadOp_ar_r<uint8_t, Destination, Source>();
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