#ifndef H_AMBER_GAMEBOY_DEBUGGER
#define H_AMBER_GAMEBOY_DEBUGGER

#include <gameboy/api.hpp>
#include <gameboy/cpu.hpp>

#include <common/debugger.hpp>

namespace Amber::Gameboy
{
	class GAMEBOY_API Debugger : public Common::Debugger
	{
		public:
		Debugger(CPU& a_CPU);
		~Debugger() noexcept override = default;

		uint64_t GetMaximumAddress() const noexcept override;

		bool IsValidAddress(uint64_t a_Address) const noexcept override;
		uint64_t GetInstructionSize(uint64_t a_Address) const override;
		std::string GetInstructionName(uint64_t a_Address) const override;

		uint8_t Load8(uint64_t a_Address) const override;

		bool HasBreakpoint(uint64_t a_Address) const noexcept override;
		void SetBreakpoint(uint64_t a_Address, bool a_Enabled) override;
		std::set<uint64_t> GetBreakpoints() const override;

		bool Run() override;
		void Step() override;
		bool Microstep() override;
		void Reset() override;

		private:
		struct InstructionInfo
		{
			Opcode::Enum m_Instruction;
			ExtendedOpcode::Enum m_ExtendedInstruction;
		};

		InstructionInfo GetInstruction(uint64_t a_Address) const;

		CPU& m_CPU;
		bool m_Break = false;
		std::set<uint64_t> m_Breakpoints;
	};
}

#endif