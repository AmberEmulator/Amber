#ifndef H_DEMU_COMMON_RAM
#define H_DEMU_COMMON_RAM

#include <common/rom.h>

namespace Demu::Common
{
	template <typename T, bool BE>
	class RAM : public ROM<T, BE>
	{
		public:
		RAM(size_t a_Size):
			ROM(a_Size)
		{
		}

		AddressMask<Address> GetAddressMask() const override
		{
			auto address_mask = ROM::GetAddressMask();
			address_mask.SetWriteBits(0, GetSize() * 8, true);
			return address_mask;
		}

		void Store8(Address a_Address, uint8_t a_Value) override
		{
			GetData()[a_Address] = a_Value;
		}
	};
}

#endif