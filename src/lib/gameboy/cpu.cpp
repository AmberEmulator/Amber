#include <gameboy/cpu.hpp>

using namespace Demu;
using namespace Common;
using namespace Gameboy;

CPU::CPU(Memory16& a_Memory, GameboyType::Enum a_GameboyType):
	m_GameboyType(a_GameboyType),
	m_Memory(a_Memory)
{
	// Initialize instruction
	for (size_t i = 0; i < 256; ++i)
	{
		m_Instructions[i] = &CPU::NotImplemented;
		m_ExtendedInstructions[i] = &CPU::NotImplemented;
	}
}

GameboyType::Enum CPU::GetGameboyType() const noexcept
{
	return m_GameboyType;
}

Memory16& CPU::GetMemory() noexcept
{
	return const_cast<Memory16&>(static_cast<const CPU*>(this)->GetMemory());
}

const Memory16& CPU::GetMemory() const noexcept
{
	return m_Memory;
}

Registers& CPU::GetRegisters() noexcept
{
	return const_cast<Registers&>(static_cast<const CPU*>(this)->GetRegisters());
}

const Registers& CPU::GetRegisters() const noexcept
{
	return m_Registers;
}

void CPU::Reset()
{
	// Initialize registers
	m_Registers.SetAF(0x01B0);
	m_Registers.SetBC(0x0013);
	m_Registers.SetDE(0x00D8);
	m_Registers.SetHL(0x014D);
	m_Registers.SetPC(EntryPoint);
	m_Registers.SetSP(0xFFFE);

	m_Memory.Store8(0xFF05, 0x00); //TIMA
	m_Memory.Store8(0xFF06, 0x00); //TMA
	m_Memory.Store8(0xFF07, 0x00); //TAC
	m_Memory.Store8(0xFF10, 0x80); //NR10
	m_Memory.Store8(0xFF11, 0xBF); //NR11
	m_Memory.Store8(0xFF12, 0xF3); //NR12
	m_Memory.Store8(0xFF14, 0xBF); //NR14
	m_Memory.Store8(0xFF16, 0x3F); //NR21
	m_Memory.Store8(0xFF17, 0x00); //NR22
	m_Memory.Store8(0xFF19, 0xBF); //NR24
	m_Memory.Store8(0xFF1A, 0x7F); //NR30
	m_Memory.Store8(0xFF1B, 0xFF); //NR31
	m_Memory.Store8(0xFF1C, 0x9F); //NR32
	m_Memory.Store8(0xFF1E, 0xBF); //NR33
	m_Memory.Store8(0xFF20, 0xFF); //NR41
	m_Memory.Store8(0xFF21, 0x00); //NR42
	m_Memory.Store8(0xFF22, 0x00); //NR43
	m_Memory.Store8(0xFF23, 0xBF); //NR30
	m_Memory.Store8(0xFF24, 0x77); //NR50
	m_Memory.Store8(0xFF25, 0xF3); //NR51

	// $F1 - GB, $F0 - SGB; NR52
	if (GetGameboyType() == GameboyType::Super)
	{
		m_Memory.Store8(0xFF26, 0xF0);
	}
	else
	{

		m_Memory.Store8(0xFF26, 0xF1);
	}

	m_Memory.Store8(0xFF40, 0x91); //LCDC
	m_Memory.Store8(0xFF42, 0x00); //SCY
	m_Memory.Store8(0xFF43, 0x00); //SCX
	m_Memory.Store8(0xFF45, 0x00); //LYC
	m_Memory.Store8(0xFF47, 0xFC); //BGP
	m_Memory.Store8(0xFF48, 0xFF); //OBP0
	m_Memory.Store8(0xFF49, 0xFF); //OBP1
	m_Memory.Store8(0xFF4A, 0x00); //WY
	m_Memory.Store8(0xFF4B, 0x00); //WX
	m_Memory.Store8(0xFFFF, 0x00); //IE
}

CPU::ExecutedInstruction CPU::ExecuteNextInstruction()
{
	ExecutedInstruction instruction;

	instruction.m_Instruction = static_cast<Instruction::Enum>(ReadNextByte());
	instruction.m_ExtendedInstruction = instruction.m_Instruction == Instruction::EXT ? static_cast<ExtendedInstruction::Enum>(0) : static_cast<ExtendedInstruction::Enum>(PeekNextByte());

	ExecuteInstruction(instruction.m_Instruction);

	return instruction;
}

void CPU::ExecuteInstruction(Instruction::Enum a_Instruction)
{
	(this->*(m_Instructions[a_Instruction]))();
	m_CycleCount += Instruction::GetCycles(a_Instruction).value();
}

void CPU::ExecuteNextExtendedInstruction()
{
	ExecuteExtendedInstruction(static_cast<ExtendedInstruction::Enum>(ReadNextByte()));
}

void CPU::ExecuteExtendedInstruction(ExtendedInstruction::Enum a_Instruction)
{
	(this->*(m_ExtendedInstructions[a_Instruction]))();
	m_CycleCount += ExtendedInstruction::GetCycleCount(a_Instruction).value();
}

uint8_t CPU::PeekNextByte() const noexcept
{
	return m_Memory.Load8(m_Registers.GetPC());
}

uint16_t CPU::PeekNextWord() const noexcept
{
	return m_Memory.Load16(m_Registers.GetPC());
}

uint8_t CPU::ReadNextByte() noexcept
{
	const uint8_t byte = PeekNextByte();
	m_Registers.SetPC(m_Registers.GetPC() + 1);
	return byte;
}

uint16_t CPU::ReadNextWord() noexcept
{
	const uint16_t word = PeekNextWord();
	m_Registers.SetPC(m_Registers.GetPC() + 2);
	return word;
}

void CPU::NotImplemented()
{
	assert(false && "This instruction has not been implemented.");
}

void CPU::NOP() noexcept
{
}