#ifndef H_AMBER_COMMON_INSTRUCTIONBUILDER
#define H_AMBER_COMMON_INSTRUCTIONBUILDER

#include <common/api.hpp>
#include <common/instructionset.hpp>

#include <memory>
#include <vector>

namespace Amber::Common
{
	template <typename Opcode, typename MicroOp, MicroOp EndOp>
	class InstructionBuilder
	{
		public:
		template <typename... Ops>
		InstructionBuilder& Begin(Opcode a_Opcode, Ops... a_Ops)
		{
			m_CurrentOp = a_Opcode;
			Allocate(m_CurrentOp);
			Clear(m_CurrentOp);
			return Cycle(a_Ops...);
		}

		template <typename... Ops>
		InstructionBuilder& Cycle(Ops... a_Ops)
		{
			if constexpr (sizeof...(Ops) > 0)
			{
				Push(m_CurrentOp, a_Ops...);
			}
			if constexpr (EndOp != nullptr)
			{
				Push(m_CurrentOp, EndOp);
			}
			return *this;
		}

		std::unique_ptr<InstructionSet<Opcode, MicroOp>> Build()
		{
			// Count the total amount of ops
			const size_t instruction_count = m_Instructions.size();
			size_t total_op_count = 0;
			for (size_t instruction_index = 0; instruction_index < instruction_count; ++instruction_index)
			{
				auto& instruction = m_Instructions[instruction_index];
				total_op_count += instruction.size();
			}

			// Allocate the instruction set
			auto instruction_set = std::make_unique<InstructionSet<Opcode, MicroOp>>(instruction_count, total_op_count);

			// Fill the instruction set
			total_op_count = 0;
			for (size_t instruction_index = 0; instruction_index < instruction_count; ++instruction_index)
			{
				auto& instruction = m_Instructions[instruction_index];
				const auto opcode = static_cast<Opcode>(instruction_index);
				const size_t op_count = instruction.size();

				instruction_set->SetInstructionOffset(opcode, total_op_count);
				instruction_set->SetInstructionSize(opcode, op_count);
				
				for (size_t op_index = 0; op_index < op_count; ++op_index)
				{
					instruction_set->SetOp(total_op_count + op_index, instruction[op_index]);
				}

				total_op_count += instruction.size();
			}

			// Return result
			return instruction_set;
		}

		void Resize(size_t a_Size)
		{
			std::vector<MicroOp> default_ops;
			if constexpr (EndOp != nullptr)
			{
				default_ops.push_back(EndOp);
			}

			m_Instructions.resize(a_Size, default_ops);
		}

		private:
		void Allocate(Opcode a_Opcode)
		{
			const size_t required_size = static_cast<size_t>(a_Opcode) + 1;
			if (m_Instructions.size() < required_size)
			{
				Resize(required_size);
			}
		}

		void Clear(Opcode a_Opcode)
		{
			m_Instructions[a_Opcode].clear();
		}

		template <typename... Ops>
		void Push(Opcode a_Opcode, MicroOp a_Op, Ops... a_Ops)
		{
			m_Instructions[a_Opcode].push_back(a_Op);

			if constexpr (sizeof...(Ops) > 0)
			{
				Push(a_Opcode, a_Ops...);
			}
		}

		std::vector<std::vector<MicroOp>> m_Instructions;
		Opcode m_CurrentOp;
	};
}

#endif