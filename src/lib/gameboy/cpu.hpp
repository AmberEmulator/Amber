#ifndef H_DEMU_GAMEBOY_CPU
#define H_DEMU_GAMEBOY_CPU

#include <gameboy/api.hpp>
#include <gameboy/extendedinstruction.hpp>
#include <gameboy/gameboytype.hpp>
#include <gameboy/instruction.hpp>
#include <gameboy/registers.hpp>

#include <common/memory.h>

namespace Demu::Gameboy
{
	class GAMEBOY_API CPU
	{
		public:
		static constexpr uint64_t ClassicClockSpeed = 0x40'0000;
		static constexpr uint64_t SuperClockSpeed = 0x41'8B1E;

		static constexpr uint16_t EntryPoint = 0x0100;

		static constexpr uint16_t InterruptRequestAddress = 0xFF0F;
		static constexpr uint16_t SpeedSwitchAddress = 0xFF4D;
		static constexpr uint16_t HRAMAddress = 0xFF80;
		static constexpr uint16_t HRAMSize = 127;
		static constexpr uint16_t InterruptEnableAddress = 0xFFFF;

		struct ExecutedInstruction
		{
			Instruction::Enum m_Instruction;
			ExtendedInstruction::Enum m_ExtendedInstruction;
		};

		CPU(Common::Memory16& a_Memory, GameboyType::Enum a_GameboyType);

		// Members
		GameboyType::Enum GetGameboyType() const noexcept;
		Common::Memory16& GetMemory() noexcept;
		const Common::Memory16& GetMemory() const noexcept;

		// Registers
		Registers& GetRegisters() noexcept;
		const Registers& GetRegisters() const noexcept;

		// Execution
		void Reset();
		ExecutedInstruction ExecuteNextInstruction();

		private:
		using InstructionCallback = void (CPU::*)() noexcept;

		// Instruction Exectution
		void ExecuteInstruction(Instruction::Enum a_Instruction);
		void ExecuteNextExtendedInstruction();
		void ExecuteExtendedInstruction(ExtendedInstruction::Enum a_Instruction);

		// Reading bytes
		uint8_t PeekNextByte() const noexcept;
		uint16_t PeekNextWord() const noexcept;
		uint8_t ReadNextByte() noexcept;
		uint16_t ReadNextWord() noexcept;

		// Operators
		using UnaryOperator8 = uint8_t(CPU::*)(uint8_t a_Value);
		using UnaryOperator16 = uint16_t(CPU::*)(uint16_t a_Value);
		using BinaryOperator8 = uint8_t(CPU::*)(uint8_t a_Left, uint8_t a_Right);
		using BinaryOperator16 = uint16_t(CPU::*)(uint16_t a_Left, uint16_t a_Right);

		template <bool Carry> uint8_t AddByte(uint8_t a_Left, uint8_t a_Right) noexcept;
		template <bool Carry> uint8_t SubtractByte(uint8_t a_Left, uint8_t a_Right) noexcept;
		uint8_t IncrementByte(uint8_t a_Value) noexcept;
		uint8_t DecrementByte(uint8_t a_Value) noexcept;
		uint16_t IncrementWord(uint16_t a_Value) noexcept;
		uint16_t DecrementWord(uint16_t a_Value) noexcept;
		uint8_t ANDByte(uint8_t a_Left, uint8_t a_Right) noexcept;
		uint8_t ORByte(uint8_t a_Left, uint8_t a_Right) noexcept;
		uint8_t XORByte(uint8_t a_Left, uint8_t a_Right) noexcept;
		
		// Instruction helpers
		template <uint8_t Flag, bool Set, InstructionCallback Callback> void Conditional() noexcept;
		template <InstructionCallback Callback, InstructionCallback... Callbacks> void Join() noexcept;

		template <uint8_t Destination, UnaryOperator8 Operator, bool Store = true> void UnaryInstruction_r() noexcept;
		template <uint8_t Destination, UnaryOperator8 Operator, bool Store = true> void UnaryInstruction_arr() noexcept;
		template <uint8_t Destination, UnaryOperator16 Operator, bool Store = true> void UnaryInstruction_rr() noexcept;

		template <uint8_t Destination, BinaryOperator8 Operator, bool Store = true> void BinaryInstruction_r_x(uint8_t a_Value) noexcept;
		template <uint8_t Destination, BinaryOperator8 Operator, bool Store = true> void BinaryInstruction_r_n() noexcept;
		template <uint8_t Destination, uint8_t Source, BinaryOperator8 Operator, bool Store = true> void BinaryInstruction_r_r() noexcept;
		template <uint8_t Destination, uint8_t Source, BinaryOperator8 Operator, bool Store = true> void BinaryInstruction_r_arr() noexcept;

		// Misc instructions
		void NotImplemented() noexcept;
		void NOP() noexcept;

		// 8-bit load instructions
		template <uint8_t Destination> void LD_r_x(uint8_t a_Value) noexcept;
		template <uint8_t Destination> void LD_r_n() noexcept;
		template <uint8_t Destination, uint8_t Source> void LD_r_r() noexcept;

		template <uint8_t Destination> void LD_r_axx(uint16_t a_Address) noexcept;
		template <uint8_t Destination> void LD_r_ann() noexcept;
		template <uint8_t Destination, uint8_t Source> void LD_r_arr() noexcept;

		template <uint8_t Destination> void LD_r_aFFx(uint8_t a_Offset) noexcept;
		template <uint8_t Destination> void LD_r_aFFn() noexcept;
		template <uint8_t Destination, uint8_t Source> void LD_r_aFFr() noexcept;

		void LD_axx_x(uint16_t a_Address, uint8_t a_Value);
		template <uint8_t Source> void LD_axx_r(uint16_t a_Address) noexcept;
		template <uint8_t Destination> void LD_arr_x(uint8_t a_Value) noexcept;
		template <uint8_t Destination> void LD_arr_n() noexcept;
		template <uint8_t Destination, uint8_t Source> void LD_arr_r() noexcept;

		template <uint8_t Source> void LD_ann_x(uint8_t a_Value) noexcept;
		template <uint8_t Source> void LD_ann_r() noexcept;

		template <uint8_t Source> void LD_aFFx_r(uint8_t a_Offset) noexcept;
		template <uint8_t Source> void LD_aFFn_r() noexcept;
		template <uint8_t Destination, uint8_t Source> void LD_aFFr_r() noexcept;

		// 16-bit load instructions
		template <uint8_t Destination> void LD_rr_xx(uint16_t a_Value) noexcept;
		template <uint8_t Destination> void LD_rr_nn() noexcept;
		template <uint8_t Destination, uint8_t Source> void LD_rr_rr() noexcept;
		template <uint8_t Destination, uint8_t Source> void LD_rr_rrn() noexcept;

		template <uint8_t Source> void LD_xx_rr(uint16_t a_Address) noexcept;
		template <uint8_t Source> void LD_ann_rr() noexcept;

		// 16-bit add instructions
		template <uint8_t Destination> void ADD_rr_xx(uint16_t a_Value) noexcept;
		template <uint8_t Destination> void ADD_rr_n() noexcept;
		template <uint8_t Destination, uint8_t Source> void ADD_rr_rr() noexcept;

		// Absolute jump instructions
		void JP_xx(uint16_t a_Address) noexcept;
		void JP_nn() noexcept;
		template <uint8_t Register> void JP_rr() noexcept;

		// Relative jump instructions
		void JR_x(uint8_t a_Offset) noexcept;
		void JR_n() noexcept;

		// Members
		const GameboyType::Enum m_GameboyType;
		Common::Memory16& m_Memory;
		Registers m_Registers;
		uint64_t m_CycleCount = 0;

		InstructionCallback m_Instructions[256];
		InstructionCallback m_ExtendedInstructions[256];
	};
}

#endif