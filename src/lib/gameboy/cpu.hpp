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

		// Breakpoints
		bool HasBreakpoint(uint16_t a_Address) const;
		void SetBreakpoint(uint16_t a_Address, bool a_Enabled);
		void SetBreakpointCallback(std::function<void()>&& a_Callback);

		// Execution
		bool Tick();
		void Reset();

		private:
		// Reading bytes
		uint8_t PeekNextByte() const noexcept;
		uint16_t PeekNextWord() const noexcept;
		uint8_t ReadNextByte() noexcept;
		uint16_t ReadNextWord() noexcept;

		// Managing Ops
		void PushOp(MicroOp a_Op);
		MicroOp PopOp();
		void InsertOP(MicroOp a_Op, size_t a_Index);
		void CallOp(MicroOp a_Op);

		// Math ops
		template <bool Carry> uint8_t AddByte(uint8_t a_Left, uint8_t a_Right) noexcept;
		template <bool Carry> uint8_t SubtractByte(uint8_t a_Left, uint8_t a_Right) noexcept;
		uint8_t IncrementByte(uint8_t a_Value) noexcept;
		uint8_t DecrementByte(uint8_t a_Value) noexcept;
		uint16_t IncrementWord(uint16_t a_Value) noexcept;
		uint16_t DecrementWord(uint16_t a_Value) noexcept;
		uint8_t ANDByte(uint8_t a_Left, uint8_t a_Right) noexcept;
		uint8_t ORByte(uint8_t a_Left, uint8_t a_Right) noexcept;
		uint8_t XORByte(uint8_t a_Left, uint8_t a_Right) noexcept;
		uint8_t SwapByte(uint8_t a_Value) noexcept;
		uint8_t RotateLeftByte(uint8_t a_Value) noexcept;
		uint8_t RotateLeftThroughCarryByte(uint8_t a_Value) noexcept;
		uint8_t RotateRightByte(uint8_t a_Value) noexcept;
		uint8_t RotateRightThroughCarryByte(uint8_t a_Value) noexcept;
		uint16_t SignedAdd(uint16_t a_Left, uint8_t a_Right) noexcept;

		// Composition ops
		using UnaryOp8 = uint8_t(CPU::*)(uint8_t a_Value);
		using UnaryOp16 = uint16_t(CPU::*)(uint16_t a_Value);
		using BinaryOp8 = uint8_t(CPU::*)(uint8_t a_Left, uint8_t a_Right);
		using BinaryOp16 = uint16_t(CPU::*)(uint16_t a_Left, uint16_t a_Right);

		template <uint8_t Destination, UnaryOp8 Op, bool Store = true> void UnaryOp_r() noexcept;
		template <uint8_t Destination, UnaryOp8 Op, bool Store = true> void UnaryOp_arr() noexcept;
		template <uint8_t Destination, UnaryOp16 Op, bool Store = true> void UnaryOp_rr() noexcept;

		template <uint8_t Destination, BinaryOp8 Op, bool Store = true> void BinaryOp_r_x(uint8_t a_Value) noexcept;
		template <uint8_t Destination, uint8_t Source, BinaryOp8 Op, bool Store = true> void BinaryOp_r_r() noexcept;
		template <uint8_t Destination, uint8_t Source, BinaryOp8 Op, bool Store = true> void BinaryOp_r_arr() noexcept;

		// Base ops
		void NotImplemented();
		void DecodeInstruction();
		void DecodeExtendedInstruction();
		void BreakpointStop();
		void BreakpointContinue();
		void Break();
		void Skip();
		template <uint8_t Flag, bool Set> void FlagCondition();

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
		template <uint8_t Destination> void ADD_rr_xx(uint16_t a_Value) noexcept;
		template <uint8_t Destination, uint8_t Source> void ADD_rr_r() noexcept;
		template <uint8_t Destination, uint8_t Source> void ADD_rr_rr() noexcept;

		// Absolute jump ops
		void JP_xx(uint16_t a_Address) noexcept;
		template <uint8_t Source> void JP_rr() noexcept;
		template <uint16_t Address> void JP() noexcept;

		// Relative jump ops
		template <uint8_t Base, uint8_t Offset> void JR_rr_r() noexcept;

		// Push ops
		void PUSH_xx(uint16_t a_Value) noexcept;
		template <uint8_t Source> void PUSH_rr() noexcept;

		// Pop ops
		uint16_t POP_xx() noexcept;
		template <uint8_t Destination> void POP_rr() noexcept;

		// Call ops
		void CALL_xx(uint16_t a_Address) noexcept;
		void CALL_nn() noexcept;

		// Return ops
		template <uint8_t Flag, bool Set> void RET() noexcept;

		// Restart ops
		template <uint16_t Address> void RST() noexcept;

		// 8-bit test bit ops
		template <uint8_t Bit> void BIT_x_b(uint8_t a_Value) noexcept;
		template <uint8_t Source, uint8_t Bit> void BIT_r_b() noexcept;
		template <uint8_t Source, uint8_t Bit> void BIT_arr_b() noexcept;

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
		bool m_OpBreak;

		// Breakpoints
		std::function<void()> m_BreakpointCallback;
	};
}

#endif