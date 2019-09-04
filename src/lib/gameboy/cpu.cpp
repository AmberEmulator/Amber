#include <gameboy/cpu.hpp>

#include <cassert>

using namespace Amber;
using namespace Common;
using namespace Gameboy;

CPU::CPU(Memory16& a_Memory):
	m_Memory(a_Memory)
{
	Reset();
}

Memory16& CPU::GetMemory() const noexcept
{
	return m_Memory;
}

void CPU::Tick()
{
	const MicroOp op = PopOp();
	(this->*op)();
}

void CPU::Reset()
{
	m_OpCount = 0;
	PushOp(&CPU::DecodeInstruction);
}

void CPU::PushOp(MicroOp a_Op)
{
	assert(m_OpCount < std::size(m_MicroOps));

	const size_t index = (m_CurrentOp + m_OpCount) % std::size(m_MicroOps);
	m_MicroOps[index] = a_Op;
	++m_OpCount;
}

CPU::MicroOp CPU::PopOp()
{
	assert(m_OpCount > 0);

	const MicroOp op = m_MicroOps[m_CurrentOp];
	m_CurrentOp = (m_CurrentOp + 1) % std::size(m_MicroOps);
	--m_OpCount;

	return op;
}

void CPU::DecodeInstruction()
{
	PushOp(&CPU::DecodeInstruction);
}