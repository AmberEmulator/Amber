#ifndef H_DEMU_COMMON_ROM
#define H_DEMU_COMMON_ROM

#include <common/memory.h>

#include <memory>

namespace Demu::Common
{
	template <typename T, bool BE>
	class ROM : public MemoryHelper<T, BE>
	{
		public:
		explicit ROM(size_t a_Size):
			m_Size(a_Size),
			m_Data(std::make_unique<uint8_t[]>(m_Size))
		{
			std::memset(m_Data.get(), 0, m_Size);
		}

		size_t GetSize() const noexcept
		{
			return m_Size;
		}

		uint8_t* GetData() noexcept
		{
			return const_cast<uint8_t*>(static_cast<const ROM*>(this)->GetData());
		}

		const uint8_t* GetData() const noexcept
		{
			return m_Data.get();
		}

		AddressMask<Address> GetAddressMask() const override
		{
			AddressMask<Address> address_mask(GetSize() * 8);
			address_mask.SetReadBits(0, GetSize() * 8, true);
			return address_mask;
		}

		uint8_t Load8(Address a_Address) const override
		{
			return GetData()[a_Address];
		}

		void Store8(Address a_Address, uint8_t a_Value) override
		{
		}

		private:
		size_t m_Size;
		std::unique_ptr<uint8_t[]> m_Data;
	};
}

#endif