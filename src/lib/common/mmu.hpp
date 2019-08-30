#ifndef H_AMBER_COMMON_MMU
#define H_AMBER_COMMON_MMU

#include <common/memory.hpp>
#include <common/memorymapping.hpp>

#include <cstdint>
#include <type_traits>
#include <vector>

namespace Amber::Common
{
	template <typename T, bool BE>
	class MMU : public MemoryHelper<T, BE>
	{
		public:
		MMU(Address a_BlockSize, size_t a_BlockCount):
			m_ReadBlocks(a_BlockCount),
			m_WriteBlocks(a_BlockCount),
			m_BlockSize(a_BlockSize)
		{
		}

		uint8_t Load8(Address a_Address) const override
		{
			const auto memory = GetReadBlock(a_Address).m_Memory;
			if (memory == nullptr)
			{
				// TODO: handle error
				return 0xFF;
			}

			return memory->Load8(a_Address % m_BlockSize);
		}

		void Store8(Address a_Address, uint8_t a_Value) override
		{
			const auto memory = GetWriteBlock(a_Address).m_Memory;
			if (memory == nullptr)
			{
				// TODO: handle error
				return;
			}

			memory->Store8(a_Address % m_BlockSize, a_Value);
		}

		void Map(Memory<Address>* a_Memory, const MemoryMapping<Address>& a_Mapping)
		{
			if (a_Mapping.GetSize() == 0)
			{
				return;
			}

			const Address start_address = a_Mapping.GetStart();
			const Address end_address = start_address + a_Mapping.GetSize();
			assert(start_address < end_address);

			const Address first_block_index = start_address / m_BlockSize;
			const Address last_block_index = start_address / m_BlockSize;

			for (Address block_index = first_block_index; block_index <= last_block_index; ++block_index)
			{
				if (a_Mapping.GetAccess() & MemoryAccess::Read)
				{
					auto& block = m_ReadBlocks[block_index];
					block.m_Memory = a_Memory;
				}

				if (a_Mapping.GetAccess() & MemoryAccess::Write)
				{
					auto& block = m_WriteBlocks[block_index];
					block.m_Memory = a_Memory;
				}
			}
		}

		private:
		struct Block
		{
			Memory<Address>* m_Memory = nullptr;
		};

		Block& GetReadBlock(Address a_Address)
		{
			return const_cast<Block&>(static_cast<const MMU*>(this)->GetReadBlock(a_Address));
		}

		const Block& GetReadBlock(Address a_Address) const
		{
			return m_ReadBlocks[a_Address / m_BlockSize];
		}

		Block& GetWriteBlock(Address a_Address)
		{
			return const_cast<Block&>(static_cast<const MMU*>(this)->GetWriteBlock(a_Address));
		}

		const Block& GetWriteBlock(Address a_Address) const
		{
			return m_WriteBlocks[a_Address / m_BlockSize];
		}

		std::vector<Block> m_ReadBlocks;
		std::vector<Block> m_WriteBlocks;
		const Address m_BlockSize;
	};
}

#endif