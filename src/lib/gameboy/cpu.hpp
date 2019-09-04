#ifndef H_AMBER_GAMEBOY_CPU
#define H_AMBER_GAMEBOY_CPU

#include <gameboy/api.hpp>
#include <gameboy/extendedinstruction.hpp>
#include <gameboy/instruction.hpp>
#include <gameboy/registers.hpp>

#include <common/memory.hpp>

namespace Amber::Gameboy
{
	class GAMEBOY_API CPU : public Registers
	{
		public:
		CPU(Common::Memory16& a_Memory);

		Common::Memory16& GetMemory() const noexcept;

		void Tick();
		void Reset();

		private:
		using MicroOp = void (CPU::*)();

		void PushOp(MicroOp a_Op);
		MicroOp PopOp();

		void DecodeInstruction();
		void NOP();

		Common::Memory16& m_Memory;

		MicroOp m_MicroOps[8];
		size_t m_CurrentOp = 0;
		size_t m_OpCount = 0;
	};
}

#endif