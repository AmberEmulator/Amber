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
		using InstructionCallback = void (CPU::*)();

		void ExecuteInstruction(Instruction::Enum a_Instruction);
		void ExecuteNextExtendedInstruction();
		void ExecuteExtendedInstruction(ExtendedInstruction::Enum a_Instruction);

		uint8_t PeekNextByte() const noexcept;
		uint16_t PeekNextWord() const noexcept;
		uint8_t ReadNextByte() noexcept;
		uint16_t ReadNextWord() noexcept;

		// Misc instructions
		void NotImplemented();
		void NOP() noexcept;

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