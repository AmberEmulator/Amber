#ifndef H_AMBER_COMMON_INSTRUCTIONSET
#define H_AMBER_COMMON_INSTRUCTIONSET

#include <common/api.hpp>

#include <memory>

namespace Amber::Common
{
	template <typename Opcode, typename MicroOp>
	class InstructionSet
	{
		public:
		InstructionSet(size_t a_InstructionCount, size_t a_OpCount):
			m_InstructionCount(a_InstructionCount),
			m_OpCount(a_OpCount)
		{
			// Calculate buffer sizes
			const size_t instruction_buffer_size = m_InstructionCount * sizeof(Instruction);
			const size_t op_buffer_size = m_OpCount * sizeof(MicroOp);

			// Make sure that op buffer has correct alignment (member pointers can be weird)
			size_t op_buffer_offset = 0;
			if (instruction_buffer_size % alignof(MicroOp) != 0)
			{
				op_buffer_offset = alignof(MicroOp) - (instruction_buffer_size % alignof(MicroOp));
			}

			// Allocate the buffer
			const size_t buffer_size = instruction_buffer_size + op_buffer_offset + op_buffer_size;
			m_Buffer = std::make_unique<uint8_t[]>(buffer_size);

			// Get pointers to the arrays
			m_Instructions = reinterpret_cast<Instruction*>(m_Buffer.get());
			m_Ops = reinterpret_cast<MicroOp*>(m_Buffer.get() + m_InstructionCount * sizeof(Instruction) + op_buffer_offset);
		}

		size_t GetInstructionCount() const noexcept
		{
			return m_InstructionCount;
		}

		size_t GetInstructionOffset(Opcode a_Opcode)
		{
			return m_Instructions[a_Opcode].m_Offset;
		}

		size_t GetInstructionSize(Opcode a_Opcode)
		{
			return m_Instructions[a_Opcode].m_Size;
		}

		const MicroOp* GetInstructionOps(Opcode a_Opcode)
		{
			return m_Ops + GetInstructionOffset(a_Opcode);
		}

		void SetInstructionOffset(Opcode a_Opcode, size_t a_Offset)
		{
			m_Instructions[a_Opcode].m_Offset = a_Offset;
		}

		void SetInstructionSize(Opcode a_Opcode, size_t a_Size)
		{
			m_Instructions[a_Opcode].m_Size = a_Size;
		}

		void SetOp(size_t a_Offset, MicroOp a_Op)
		{
			m_Ops[a_Offset] = a_Op;
		}

		private:
		struct Instruction
		{
			size_t m_Offset;
			size_t m_Size;
		};

		size_t m_InstructionCount;
		size_t m_OpCount;
		Instruction* m_Instructions;
		MicroOp* m_Ops;
		std::unique_ptr<uint8_t[]> m_Buffer;
	};
}

#endif