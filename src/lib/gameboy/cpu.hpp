#ifndef H_AMBER_GAMEBOY_CPU
#define H_AMBER_GAMEBOY_CPU

#include <gameboy/api.hpp>
#include <gameboy/extendedopcode.hpp>
#include <gameboy/opcode.hpp>

#include <common/cpuhelper.hpp>
#include <common/instructionset.hpp>
#include <common/memory.hpp>
#include <common/register.hpp>

#include <functional>

namespace Amber::Gameboy
{
	class GAMEBOY_API CPU : public Common::CPUHelper<CPU, uint16_t, 8, 5>
	{
		public:
		static constexpr uint8_t RegisterAF = 0;
		static constexpr uint8_t RegisterBC = 1;
		static constexpr uint8_t RegisterDE = 2;
		static constexpr uint8_t RegisterHL = 3;
		static constexpr uint8_t RegisterSP = 4;
		static constexpr uint8_t RegisterPC = 5;
		static constexpr uint8_t RegisterXY = 6;
		static constexpr uint8_t RegisterZW = 7;

		static constexpr uint8_t RegisterA = (RegisterAF << 1) + 0;
		static constexpr uint8_t RegisterF = (RegisterAF << 1) + 1;
		static constexpr uint8_t RegisterB = (RegisterBC << 1) + 0;
		static constexpr uint8_t RegisterC = (RegisterBC << 1) + 1;
		static constexpr uint8_t RegisterD = (RegisterDE << 1) + 0;
		static constexpr uint8_t RegisterE = (RegisterDE << 1) + 1;
		static constexpr uint8_t RegisterH = (RegisterHL << 1) + 0;
		static constexpr uint8_t RegisterL = (RegisterHL << 1) + 1;
		static constexpr uint8_t RegisterSP_S = (RegisterSP << 1) + 0;
		static constexpr uint8_t RegisterSP_P = (RegisterSP << 1) + 1;
		static constexpr uint8_t RegisterPC_P = (RegisterPC << 1) + 0;
		static constexpr uint8_t RegisterPC_C = (RegisterPC << 1) + 1;
		static constexpr uint8_t RegisterX = (RegisterXY << 1) + 0;
		static constexpr uint8_t RegisterY = (RegisterXY << 1) + 1;
		static constexpr uint8_t RegisterZ = (RegisterZW << 1) + 0;
		static constexpr uint8_t RegisterW = (RegisterZW << 1) + 1;

		static constexpr uint8_t FlagZero = 7;
		static constexpr uint8_t FlagSubtract = 6;
		static constexpr uint8_t FlagHalfCarry = 5;
		static constexpr uint8_t FlagCarry = 4;

		static constexpr uint8_t InterruptVBlank  = 0x01;
		static constexpr uint8_t InterruptLCDSTAT = 0x02;
		static constexpr uint8_t InterruptTimer   = 0x04;
		static constexpr uint8_t InterruptSerial  = 0x08;
		static constexpr uint8_t InterruptJoypad  = 0x10;

		// Constructors
		CPU(Common::Memory16& a_Memory);

		// Memory
		Common::Memory16& GetMemory() const noexcept;

		// Registers
		bool LoadFlag(uint8_t a_Flag) const noexcept;
		void StoreFlag(uint8_t a_Flag, bool a_Value) noexcept;

		// Interrupts
		uint8_t GetInterruptEnable() const noexcept;
		uint8_t GetInterruptRequests() const noexcept;

		void SetInterruptEnable(uint8_t a_Interrupts) noexcept;
		void SetInterruptRequests(uint8_t a_Interrupts) noexcept;

		void RequestInterrupts(uint8_t a_Interrupts) noexcept;

		// DMA
		void StartDMA(uint8_t a_Address);

		// Breakpoints
		bool HasBreakpoint(uint16_t a_Address) const;
		void SetBreakpoint(uint16_t a_Address, bool a_Enabled);
		void SetBreakpointCallback(std::function<void()>&& a_Callback);

		// Execution
		bool Tick();
		void Reset();

		private:
		// Math ops
		template <bool Carry> uint8_t Add8(uint8_t a_Left, uint8_t a_Right) noexcept;
		template <bool Carry> uint8_t Subtract8(uint8_t a_Left, uint8_t a_Right) noexcept;
		uint16_t Add16(uint16_t a_Left, uint16_t a_Right) noexcept;
		uint8_t Increment8(uint8_t a_Value) noexcept;
		uint8_t Decrement8(uint8_t a_Value) noexcept;
		uint16_t Increment16(uint16_t a_Value) noexcept;
		uint16_t Decrement16(uint16_t a_Value) noexcept;
		uint8_t AND8(uint8_t a_Left, uint8_t a_Right) noexcept;
		uint8_t OR8(uint8_t a_Left, uint8_t a_Right) noexcept;
		uint8_t XOR8(uint8_t a_Left, uint8_t a_Right) noexcept;
		uint8_t Swap8(uint8_t a_Value) noexcept;
		uint8_t Complement8(uint8_t a_Value) noexcept;
		template <bool ResetZero = false> uint8_t RotateLeft8(uint8_t a_Value) noexcept;
		template <bool ResetZero = false> uint8_t RotateLeftThroughCarry8(uint8_t a_Value) noexcept;
		template <bool ResetZero = false> uint8_t RotateRight8(uint8_t a_Value) noexcept;
		template <bool ResetZero = false> uint8_t RotateRightThroughCarry8(uint8_t a_Value) noexcept;
		template <bool FillWithZero> uint8_t ShiftLeft8(uint8_t a_Value);
		template <bool FillWithZero> uint8_t ShiftRight8(uint8_t a_Value);
		template <uint8_t Bit> uint8_t SetBit8(uint8_t a_Value);
		template <uint8_t Bit> uint8_t ResetBit8(uint8_t a_Value);
		template <bool Flags = true> uint16_t SignedAdd16(uint16_t a_Left, uint8_t a_Right) noexcept;
		uint8_t DecimalAdjust8(uint8_t a_Value) noexcept;

		// Base ops
		void NotImplemented();
		void DecodeInstruction();
		void DecodeExtendedInstruction();
		void BreakpointStop();
		void BreakpointContinue();
		void Break();
		template <uint8_t Flag, bool Set> void FlagCondition();
		void DisableInterrupts();
		void EnableInterrupts();
		void ProcessInterrupts();
		void Halt();
		void CheckHalt();
		template <uint8_t Destination, uint8_t Mask> void MaskOp_r8();
		void DMA();

		// 16-bit load ops
		template <uint8_t Destination, uint8_t Source> void LoadOp_r16_x16r8(uint16_t a_Base);
		template <uint8_t Destination, uint8_t Source> void LoadOp_r16_FFr8();
		template <uint8_t Destination, uint8_t BaseSource, uint8_t OffsetSource> void LoadOp_r16_r16r8();

		// 16-bit add ops
		template <uint8_t Destination, uint8_t Source> void AddOp_r16_r8() noexcept;

		// Relative jump ops
		template <uint8_t Base, uint8_t Offset> void JumpOp_r16_r8() noexcept;

		// 8-bit test bit ops
		template <uint8_t Bit> void BitTestOp_x8_b(uint8_t a_Value) noexcept;
		template <uint8_t Source, uint8_t Bit> void BitTestOp_r8_b() noexcept;

		// Flag ops
		void CCF() noexcept;
		void SCF() noexcept;

		// Memory
		Common::Memory16& m_Memory;

		// Instructions
		std::unique_ptr<Common::InstructionSet<Opcode::Enum, MicroOp>> m_Instructions;
		std::unique_ptr<Common::InstructionSet<ExtendedOpcode::Enum, MicroOp>> m_ExtendedInstructions;

		// Opcode queue
		bool m_OpBreak = false;

		// Interrupts
		bool m_InterruptMasterEnable = false;
		uint8_t m_InterruptEnable = 0;
		uint8_t m_InterruptRequests = 0;
		bool m_Halted = false;

		// DMA
		uint8_t m_DMAAddress = 0;
		uint8_t m_DMACounter = 0;

		// Breakpoints
		std::function<void()> m_BreakpointCallback;
	};
}

#endif