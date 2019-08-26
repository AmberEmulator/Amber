#ifndef H_AMBER_COMMON_MMU
#define H_AMBER_COMMON_MMU

#include <common/memory.h>

#include <cstdint>
#include <type_traits>
#include <vector>

namespace Amber::Common
{
	template <typename T, bool BE>
	class MMU : public MemoryHelper<T, BE>
	{
		public:
		MMU(Address a_BlockSize, size_t a_BlockCount) :
			m_Blocks(a_BlockCount),
			m_BlockSize(a_BlockSize)
		{
		}

		AddressMask<Address> GetAddressMask() const override
		{
			return AddressMask<Address>(0);
		}

		uint8_t Load8(Address a_Address) const override
		{
			return GetBlock(a_Address).m_Memory->Load8(a_Address % m_BlockSize);
		}

		void Store8(Address a_Address, uint8_t a_Value) override
		{
			GetBlock(a_Address).m_Memory->Store8(a_Address % m_BlockSize, a_Value);
		}

		void Register(Memory<Address>& a_Memory, Address a_Offset)
		{

		}

		private:
		struct Block
		{
			Memory<Address>* m_Memory = nullptr;
		};

		Block& GetBlock(Address a_Address)
		{
			return const_cast<Block&>(static_cast<const MMU*>(this)->GetBlock(a_Address));
		}

		const Block& GetBlock(Address a_Address) const
		{
			return m_Blocks[a_Address / m_BlockSize];
		}

		std::vector<Block> m_Blocks;
		const Address m_BlockSize;
	};
}

#endif