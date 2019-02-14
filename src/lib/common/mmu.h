#ifndef H_DEMU_COMMON_MMU
#define H_DEMU_COMMON_MMU

#include <common/memory.h>

#include <cstdint>
#include <type_traits>

namespace Demu::Common
{
	template <typename T>
	class MMU : public MemoryHelper<T>
	{
		using Address = T;
		static_assert(std::is_unsigned_v<Address> && !std::is_same_v<Address, bool>, "Address must be an unsigned integer");

		AddressMask<Address> GetAddressMask() const override;

		uint8_t Load8(Address a_Address) const override;
		void Store8(Address a_Address, uint8_t a_Value) override;

		void Register(Memory<T>& a_Memory, Address a_Offset);

		private:
	};
}

#endif