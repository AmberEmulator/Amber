#ifndef H_DEMU_GAMEBOY_DISASSEMBLY
#define H_DEMU_GAMEBOY_DISASSEMBLY

#include <gameboy/api.hpp>
#include <gameboy/cpu.hpp>

#include <common/disassembly.hpp>

namespace Demu::Gameboy
{
	class GAMEBOY_API Disassembly : public Common::Disassembly
	{
		public:
		Disassembly(CPU& a_CPU);
		~Disassembly() noexcept override = default;

		uint64_t GetMaximumAddress() const noexcept override;

		bool IsValidAddress(uint64_t a_Address) const noexcept override;
		uint64_t GetInstructionSize(uint64_t a_Address) const override;
		std::string GetInstructionName(uint64_t a_Address) const override;

		bool HasBreakpoint(uint64_t a_Address) const noexcept override;
		void SetBreakpoint(uint64_t a_Address, bool a_Enabled) override;

		private:
		struct InstructionInfo
		{
			Instruction::Enum m_Instruction;
			ExtendedInstruction::Enum m_ExtendedInstruction;
		};

		InstructionInfo GetInstruction(uint64_t a_Address) const;

		CPU& m_CPU;
	};
}

#endif