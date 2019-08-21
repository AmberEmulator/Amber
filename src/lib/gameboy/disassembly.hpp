#ifndef H_DEMU_GAMEBOY_DISASSEMBLY
#define H_DEMU_GAMEBOY_DISASSEMBLY

#include <gameboy/api.hpp>
#include <gameboy/extendedinstruction.hpp>
#include <gameboy/instruction.hpp>

#include <common/disassembly.hpp>
#include <common/memory.h>

namespace Demu::Gameboy
{
	class GAMEBOY_API Disassembly : public Common::Disassembly
	{
		public:
		Disassembly(const Common::Memory16& a_Memory);
		~Disassembly() noexcept override = default;

		uint64_t GetMaximumAddress() const noexcept override;

		bool IsValidAddress(uint64_t a_Address) const noexcept override;
		uint64_t GetInstructionSize(uint64_t a_Address) const override;
		std::string GetInstructionName(uint64_t a_Address) const override;

		private:
		struct InstructionInfo
		{
			Instruction::Enum m_Instruction;
			ExtendedInstruction::Enum m_ExtendedInstruction;
		};

		InstructionInfo GetInstruction(uint64_t a_Address) const;

		const Common::Memory16& m_Memory;
	};
}

#endif