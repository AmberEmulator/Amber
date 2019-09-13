#ifndef H_AMBER_GAMEBOY_CPU
#define H_AMBER_GAMEBOY_CPU

#include <gameboy/api.hpp>
#include <gameboy/extendedopcode.hpp>
#include <gameboy/instructionbuilder.hpp>
#include <gameboy/instructionset.hpp>
#include <gameboy/opcode.hpp>
#include <gameboy/register.hpp>

#include <common/memory.hpp>

#include <functional>

namespace Amber::Gameboy
{
	class GAMEBOY_API CPU
	{
		template <typename T> friend class InstructionBuilder;

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
		uint8_t LoadRegister8(uint8_t a_Register) const noexcept;
		uint16_t LoadRegister16(uint8_t a_Register) const noexcept;
		bool LoadFlag(uint8_t a_Flag) const noexcept;

		void StoreRegister8(uint8_t a_Register, uint8_t a_Value) noexcept;
		void StoreRegister16(uint8_t a_Register, uint16_t a_Value) noexcept;
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
		// Reading bytes
		uint8_t PeekNext8() const noexcept;
		uint16_t PeekNext16() const noexcept;
		uint8_t ReadNext8() noexcept;
		uint16_t ReadNext16() noexcept;

		// Managing Ops
		void PushOp(MicroOp a_Op);
		MicroOp PopOp();
		void InsertOp(MicroOp a_Op, size_t a_Index);
		void InsertOpAndIncrementDone(MicroOp a_Op);
		void CallOp(MicroOp a_Op);

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

		// Composition ops
		using UnaryOp8 = uint8_t(CPU::*)(uint8_t a_Value);
		using UnaryOp16 = uint16_t(CPU::*)(uint16_t a_Value);
		using BinaryOp8 = uint8_t(CPU::*)(uint8_t a_Left, uint8_t a_Right);
		using BinaryOp16 = uint16_t(CPU::*)(uint16_t a_Left, uint16_t a_Right);

		template <uint8_t Destination, UnaryOp8 Op, bool Store = true> void UnaryOp_r() noexcept;
		template <uint8_t Destination, UnaryOp16 Op, bool Store = true> void UnaryOp_rr() noexcept;

		template <uint8_t Destination, BinaryOp8 Op, bool Store = true> void BinaryOp_r_x(uint8_t a_Value) noexcept;
		template <uint8_t Destination, uint8_t Source, BinaryOp8 Op, bool Store = true> void BinaryOp_r_r() noexcept;
		template <uint8_t Destination, BinaryOp16 Op, bool Store = true> void BinaryOp_rr_xx(uint16_t a_Value) noexcept;
		template <uint8_t Destination, uint8_t Source, BinaryOp16 Op, bool Store = true> void BinaryOp_rr_rr() noexcept;

		template <MicroOp Op, uint8_t Counter> void Delay();

		// Base ops
		void NotImplemented();
		void DecodeInstruction();
		void DecodeExtendedInstruction();
		void BreakpointStop();
		void BreakpointContinue();
		void Break();
		void Skip();
		template <uint8_t Flag, bool Set> void FlagCondition();
		void DisableInterrupts();
		void EnableInterrupts();
		void ProcessInterrupts();
		template <uint8_t Destination, uint8_t Mask> void MASK_r();
		void DMA();

		// 8-bit load ops
		template <uint8_t Destination> void LD_r_x(uint8_t a_Value);
		template <uint8_t Destination> void LD_r_n();
		template <uint8_t Destination, uint8_t Source> void LD_r_r();
		template <uint8_t Destination> void LD_r_axx(uint16_t a_Address);
		template <uint8_t Destination, uint8_t Source> void LD_r_arr();

		void LD_axx_x(uint16_t a_Address, uint8_t a_Value);
		template <uint8_t Destination> void LD_arr_x(uint8_t a_Value);
		template <uint8_t Destination, uint8_t Source> void LD_arr_r();

		// 16-bit load ops
		template <uint8_t Destination> void LD_rr_xx(uint16_t a_Value);
		template <uint8_t Destination, uint8_t Source> void LD_rr_rr();
		template <uint8_t Destination, uint8_t Source> void LD_rr_xxr(uint16_t a_Base);
		template <uint8_t Destination, uint8_t Source> void LD_rr_FFr();
		template <uint8_t Destination, uint8_t BaseSource, uint8_t OffsetSource> void LD_rr_rrr();

		// 16-bit add ops
		template <uint8_t Destination, uint8_t Source> void ADD_rr_r() noexcept;

		// Absolute jump ops
		void JP_xx(uint16_t a_Address) noexcept;
		template <uint8_t Source> void JP_rr() noexcept;
		template <uint16_t Address> void JP() noexcept;

		// Relative jump ops
		template <uint8_t Base, uint8_t Offset> void JR_rr_r() noexcept;

		// 8-bit test bit ops
		template <uint8_t Bit> void BIT_x_b(uint8_t a_Value) noexcept;
		template <uint8_t Source, uint8_t Bit> void BIT_r_b() noexcept;

		// Flag ops
		void CCF() noexcept;
		void SCF() noexcept;

		// Memory
		Common::Memory16& m_Memory;

		// Registers
		Register m_Registers[8]; // AF, BC, DE, HL, SP, PC, XY, ZW

		// Instructions
		std::unique_ptr<InstructionSet<Opcode::Enum, MicroOp>> m_Instructions;
		std::unique_ptr<InstructionSet<ExtendedOpcode::Enum, MicroOp>> m_ExtendedInstructions;

		// Opcode queue
		MicroOp m_MicroOps[16] = {};
		size_t m_OpFront = 0;
		size_t m_OpBack = 0;
		size_t m_OpDone = 0;
		bool m_OpBreak = false;

		// Interrupts
		bool m_InterruptMasterEnable = false;
		uint8_t m_InterruptEnable = 0;
		uint8_t m_InterruptRequests = 0;

		// DMA
		uint8_t m_DMAAddress = 0;
		uint8_t m_DMACounter = 0;

		// Breakpoints
		std::function<void()> m_BreakpointCallback;
	};
}

#endif