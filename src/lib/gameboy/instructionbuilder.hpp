#ifndef H_AMBER_GAMEBOY_INSTRUCTIONBUILDER
#define H_AMBER_GAMEBOY_INSTRUCTIONBUILDER

#include <gameboy/api.hpp>
#include <gameboy/cpu.hpp>
#include <gameboy/instructionset.hpp>

#include <memory>
#include <vector>

namespace Amber::Gameboy
{
	using MicroOp = void (CPU::*)();

	template <typename Opcode>
	class GAMEBOY_API InstructionBuilder
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
			Push(m_CurrentOp, &CPU::Break);
			return *this;
		}

		std::unique_ptr<InstructionSet<Opcode, MicroOp>> Build()
		{
			// Count the total amount of ops
			const size_t instruction_count = m_Instructions.size();
			size_t op_count = 0;
			for (size_t instruction_index = 0; instruction_index < instruction_count; ++instruction_index)
			{
				auto& instruction = m_Instructions[instruction_index];
				instruction.back() = &CPU::Done;
				instruction.push_back(&CPU::Break);

				op_count += instruction.size();
			}

			// Allocate the instruction set
			auto instruction_set = std::make_unique<InstructionSet<Opcode, MicroOp>>(instruction_count, op_count);

			// Fill the instruction set
			op_count = 0;
			for (size_t instruction_index = 0; instruction_index < instruction_count; ++instruction_index)
			{
				auto& instruction = m_Instructions[instruction_index];
				const auto opcode = static_cast<Opcode>(instruction_index);

				instruction_set->SetInstructionOffset(opcode, op_count);
				instruction_set->SetInstructionSize(opcode, instruction.size());
				
				for (size_t op_index = 0; op_index < instruction.size(); ++op_index)
				{
					instruction_set->SetOp(op_count + op_index, instruction[op_index]);
				}

				op_count += instruction.size();
			}

			// Return result
			return instruction_set;
		}

		void Resize(size_t a_Size)
		{
			m_Instructions.resize(a_Size, std::vector<MicroOp>(1, &CPU::Break));
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