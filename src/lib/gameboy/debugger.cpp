#include <gameboy/debugger.hpp>

#include <gameboy/cpu.hpp>
#include <gameboy/device.hpp>
#include <gameboy/mmu.hpp>
#include <gameboy/ppu.hpp>

#include <limits>

using namespace Amber;
using namespace Gameboy;

Debugger::Debugger(Device& a_Device):
	m_Device(a_Device)
{
	m_Device.GetCPU().SetBreakpointCallback([&] { m_Break = true; });
}

uint64_t Debugger::GetMaximumAddress() const noexcept
{
	return static_cast<uint64_t>(std::numeric_limits<uint16_t>::max());
}

bool Debugger::IsValidAddress(uint64_t a_Address) const noexcept
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
	auto& memory = m_Device.GetCPU().GetMemory();

	if (memory.Load8(address - 1) == Opcode::EXT)
	{
		return false;
	}

	return true;
}

uint64_t Debugger::GetInstructionSize(uint64_t a_Address) const
{
	const auto instruction = GetInstruction(a_Address);

	if (instruction.m_Instruction != Opcode::EXT)
	{
		return Opcode::GetSize(instruction.m_Instruction).value();
	}
	else
	{
		return ExtendedOpcode::GetSize(instruction.m_ExtendedInstruction).value() + 1;
	}
}

std::string Debugger::GetInstructionName(uint64_t a_Address) const
{
	const auto instruction = GetInstruction(a_Address);

	if (instruction.m_Instruction != Opcode::EXT)
	{
		return std::string(Opcode::ToString(instruction.m_Instruction).value());
	}
	else
	{
		return std::string(ExtendedOpcode::ToString(instruction.m_ExtendedInstruction).value());
	}
}

uint8_t Debugger::Load8(uint64_t a_Address) const
{
	const uint16_t address = static_cast<uint16_t>(a_Address);
	return m_Device.GetMMU().Load8(address);
}

bool Debugger::HasBreakpoint(uint64_t a_Address) const noexcept
{
	const uint16_t address = static_cast<uint16_t>(a_Address);
	return m_Device.GetCPU().HasBreakpoint(address);
}

void Debugger::SetBreakpoint(uint64_t a_Address, bool a_Enabled)
{
	const uint16_t address = static_cast<uint16_t>(a_Address);
	m_Device.GetCPU().SetBreakpoint(address, a_Enabled);
	
	if (a_Enabled)
	{
		m_Breakpoints.emplace(a_Address);
	}
	else
	{
		m_Breakpoints.erase(a_Address);
	}
}

std::set<uint64_t> Debugger::GetBreakpoints() const
{
	return m_Breakpoints;
}

bool Debugger::Run()
{
	m_Break = false;

	// TODO: timing cycles
	for (size_t i = 0; i < PPU::FrameCycles; ++i)
	{
		Step();
		if (m_Break)
		{
			return false;
		}
	}

	return true;
}

void Debugger::Step()
{
	while (!Microstep())
	{
	}
}

bool Debugger::Microstep()
{
	return m_Device.Tick();
}

void Debugger::Reset()
{
	m_Device.Reset();
}

Debugger::InstructionInfo Debugger::GetInstruction(uint64_t a_Address) const
{
	auto& memory = m_Device.GetMMU();

	InstructionInfo instruction;

	// Cast address to correct size
	const uint16_t address = static_cast<uint16_t>(a_Address);

	// Read the instruction
	instruction.m_Instruction = static_cast<Opcode::Enum>(memory.Load8(address));

	// Check if the instruction was replaced with an interrupt
	if (instruction.m_Instruction == Opcode::BREAKPOINT_STOP || instruction.m_Instruction == Opcode::BREAKPOINT_CONTINUE)
	{
		instruction.m_Instruction = static_cast<Opcode::Enum>(memory.GetReplaced8(address).value_or(instruction.m_Instruction));
	}

	// Check if it is an extended instruction
	if (instruction.m_Instruction == Opcode::EXT)
	{
		// Read the extended instruction
		instruction.m_ExtendedInstruction = static_cast<ExtendedOpcode::Enum>(memory.Load8(address + 1));
	}

	return instruction;
}