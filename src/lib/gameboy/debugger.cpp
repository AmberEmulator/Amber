#include <gameboy/disassembly.hpp>

#include <limits>

using namespace Demu;
using namespace Gameboy;

Disassembly::Disassembly(CPU& a_CPU):
	m_CPU(a_CPU)
{
}

uint64_t Disassembly::GetMaximumAddress() const noexcept
{
	return static_cast<uint64_t>(std::numeric_limits<uint16_t>::max());
}

bool Disassembly::IsValidAddress(uint64_t a_Address) const noexcept
{
	if (a_Address > 0xFFFF)
	{
		return false;
	}

	if (a_Address == 0)
	{
		return true;
	}

	const uint16_t address = static_cast<uint16_t>(a_Address);
	auto& memory = m_CPU.GetMemory();

	if (memory.Load8(address - 1) == Instruction::EXT)
	{
		return false;
	}

	return true;
}

uint64_t Disassembly::GetInstructionSize(uint64_t a_Address) const
{
	const auto instruction = GetInstruction(a_Address);

	if (instruction.m_Instruction != Instruction::EXT)
	{
		return Instruction::GetSize(instruction.m_Instruction).value();
	}
	else
	{
		return ExtendedInstruction::GetSize(instruction.m_ExtendedInstruction).value() + 1;
	}
}

std::string Disassembly::GetInstructionName(uint64_t a_Address) const
{
	const auto instruction = GetInstruction(a_Address);

	if (instruction.m_Instruction != Instruction::EXT)
	{
		return std::string(Instruction::ToString(instruction.m_Instruction).value());
	}
	else
	{
		return std::string(ExtendedInstruction::ToString(instruction.m_ExtendedInstruction).value());
	}
}

bool Disassembly::HasBreakpoint(uint64_t a_Address) const noexcept
{
	const uint16_t address = static_cast<uint16_t>(a_Address);
	return m_CPU.HasBreakpoint(address);
}

void Disassembly::SetBreakpoint(uint64_t a_Address, bool a_Enabled)
{
	const uint16_t address = static_cast<uint16_t>(a_Address);
	m_CPU.SetBreakpoint(address, a_Enabled);
}

Disassembly::InstructionInfo Disassembly::GetInstruction(uint64_t a_Address) const
{
	auto& memory = m_CPU.GetMemory();

	InstructionInfo instruction;

	// Cast address to correct size
	const uint16_t address = static_cast<uint16_t>(a_Address);

	// Read the instruction
	instruction.m_Instruction = static_cast<Instruction::Enum>(memory.Load8(address));

	// Check if the instruction was replaced with an interrupt
	if (instruction.m_Instruction == Instruction::BREAKPOINT)
	{
		instruction.m_Instruction = static_cast<Instruction::Enum>(memory.GetReplaced8(address).value_or(instruction.m_Instruction));
	}

	// Check if it is an extended instruction
	if (instruction.m_Instruction == Instruction::EXT)
	{
		// Read the extended instruction
		instruction.m_ExtendedInstruction = static_cast<ExtendedInstruction::Enum>(memory.Load8(address + 1));
	}

	return instruction;
}