#ifndef H_AMBER_GAMEBOY_DEBUGGER
#define H_AMBER_GAMEBOY_DEBUGGER

#include <gameboy/api.hpp>

#include <gameboy/opcode.hpp>
#include <gameboy/extendedopcode.hpp>
#include <gameboy/ppuobserver.hpp>

#include <common/debugger.hpp>

namespace Amber::Gameboy
{
	class Device;

	class GAMEBOY_API Debugger : public Common::Debugger, PPUObserver
	{
		public:
		Debugger(Device& a_Device);
		~Debugger() noexcept override = default;

		uint64_t GetMaximumAddress() const noexcept override;

		bool IsValidAddress(uint64_t a_Address) const noexcept override;
		uint64_t GetInstructionSize(uint64_t a_Address) const override;
		std::string GetInstructionName(uint64_t a_Address) const override;

		uint8_t Load8(uint64_t a_Address) const override;

		// Events
		size_t GetEventCount() const noexcept override;
		std::string GetEventName(size_t a_Event) const override;

		// Execution
		bool Run() override;
		bool Step() override;
		bool Microstep() override;
		bool Reset() override;

		void OnLCDModeChange(LCDMode::Enum a_From, LCDMode::Enum a_To) override;

		protected:
		void OnBreakpointCreate(Common::Breakpoint a_Breakpoint) override;
		void OnBreakpointDestroy(Common::Breakpoint a_Breakpoint) noexcept override;

		private:
		struct InstructionInfo
		{
			Opcode::Enum m_Instruction;
			ExtendedOpcode::Enum m_ExtendedInstruction;
		};

		InstructionInfo GetInstruction(uint64_t a_Address) const;
		bool CheckBreakpoints() const;

		Device& m_Device;
		size_t m_Cycles = 0;
		bool m_EnteredVBlank = false;
		bool m_Break = false;
	};
}

#endif