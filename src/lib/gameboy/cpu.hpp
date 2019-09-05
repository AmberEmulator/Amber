#ifndef H_AMBER_GAMEBOY_CPU
#define H_AMBER_GAMEBOY_CPU

#include <gameboy/api.hpp>
#include <gameboy/extendedopcode.hpp>
#include <gameboy/instructionset.hpp>
#include <gameboy/opcode.hpp>
#include <gameboy/registers.hpp>

#include <common/memory.hpp>

namespace Amber::Gameboy
{
	class GAMEBOY_API CPU : public Registers
	{
		template <typename T> friend class InstructionBuilder;

		public:
		CPU(Common::Memory16& a_Memory);

		Common::Memory16& GetMemory() const noexcept;

		bool Tick();
		void Reset();

		private:
		using MicroOp = void (CPU::*)();
		struct Instruction
		{
			MicroOp m_MicroOps[8];
		};

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
		using UnaryOp8 = uint8_t(CPU::*)(uint8_t a_Value);
		using UnaryOp16 = uint16_t(CPU::*)(uint16_t a_Value);
		using BinaryOp8 = uint8_t(CPU::*)(uint8_t a_Left, uint8_t a_Right);
		using BinaryOp16 = uint16_t(CPU::*)(uint16_t a_Left, uint16_t a_Right);

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

		// Composition ops
		template <uint8_t Destination, UnaryOp8 Op, bool Store = true> void UnaryOp_r() noexcept;
		template <uint8_t Destination, UnaryOp8 Op, bool Store = true> void UnaryOp_c() noexcept;
		template <uint8_t Destination, UnaryOp16 Op, bool Store = true> void UnaryOp_rr() noexcept;

		template <uint8_t Destination, BinaryOp8 Op, bool Store = true> void BinaryOp_r_x(uint8_t a_Value) noexcept;
		template <uint8_t Destination, uint8_t Source, BinaryOp8 Op, bool Store = true> void BinaryOp_r_r() noexcept;
		template <uint8_t Destination, uint8_t Source, BinaryOp8 Op, bool Store = true> void BinaryOp_r_c() noexcept;
		template <uint8_t Destination, uint8_t Source, BinaryOp8 Op, bool Store = true> void BinaryOp_r_arr() noexcept;

		// Base ops
		void DecodeInstruction();
		void DecodeExtendedInstruction();
		void Break();
		void Skip();
		template <uint8_t Flag, bool Set> void FlagCondition();

		// 8-bit load ops
		template <uint8_t Destination> void LD_r_x(uint8_t a_Value);
		template <uint8_t Destination, uint8_t Source> void LD_r_r();
		template <uint8_t Destination, uint8_t Source> void LD_r_c();
		void LD_axx_x(uint16_t a_Address, uint8_t a_Value);
		template <uint8_t Destination> void LD_arr_x(uint8_t a_Value);
		template <uint8_t Destination, uint8_t Source> void LD_arr_r();
		template <uint8_t Destination, uint8_t Source> void LD_arr_c();
		template <uint8_t Destination> void LD_rr_xx(uint16_t a_Value);
		template <uint8_t Destination> void LD_rr_cc();
		template <uint8_t Destination, uint8_t Source> void LD_rr_rr();

		template <uint8_t Destination> void LD_c_x(uint8_t a_Value);
		template <uint8_t Destination, uint8_t Source> void LD_c_r();
		template <uint8_t Destination> void LD_c_n();
		template <uint8_t Destination> void LD_c_axx(uint16_t a_Address);
		template <uint8_t Destination, uint8_t Source> void LD_c_arr();
		template <uint8_t Destination> void LD_c_acc();

		// 16-bit load ops
		void LD_cc_xx(uint16_t a_Value);
		template <uint8_t Source> void LD_cc_rr();
		void LD_cc_FFx(uint8_t a_Offset);
		template <uint8_t Source> void LD_cc_FFc();
		template <uint8_t Source> void LD_cc_rrx(uint8_t a_Offset);
		template <uint8_t SourceRegister, uint8_t SourceCache> void LD_cc_rrc();
		void LD_acc_x(uint8_t a_Value);
		template <uint8_t Source> void LD_acc_r();
		void LD_axx_xx(uint16_t a_Address, uint16_t a_Value);
		void LD_acc_xx(uint16_t a_Value);
		template <uint8_t Source> void LD_acc_rr();

		// 16-bit add ops
		template <uint8_t Destination> void ADD_rr_xx(uint16_t a_Value) noexcept;
		template <uint8_t Destination, uint8_t Source> void ADD_rr_c() noexcept;
		template <uint8_t Destination, uint8_t Source> void ADD_rr_rr() noexcept;

		// Absolute jump ops
		void JP_xx(uint16_t a_Address) noexcept;
		void JP_cc() noexcept;
		template <uint8_t Source> void JP_rr() noexcept;

		// Relative jump ops
		void JR_x(uint8_t a_Offset) noexcept;
		template <uint8_t Source> void JR_c() noexcept;

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

		// Instructions
		std::unique_ptr<InstructionSet<Opcode::Enum, MicroOp>> m_Instructions;
		std::unique_ptr<InstructionSet<ExtendedOpcode::Enum, MicroOp>> m_ExtendedInstructions;

		Common::Memory16& m_Memory;

		// Opcode queue
		MicroOp m_MicroOps[16] = {};
		size_t m_OpFront = 0;
		size_t m_OpBack = 0;
		size_t m_OpDone = 0;
		Register m_OpCache;
		bool m_OpBreak;
	};
}

#endif