#include <gameboy/cpu.hpp>

using namespace Amber;
using namespace Common;
using namespace Gameboy;

CPU::CPU(Memory16& a_Memory):
	m_Memory(a_Memory)
{
}

Memory16& CPU::GetMemory() const noexcept
{
	return m_Memory;
}

void CPU::Tick()
{

}