#ifndef H_AMBER_COMMON_RAM
#define H_AMBER_COMMON_RAM

#include <common/rom.hpp>

namespace Amber::Common
{
	template <typename T, bool BE>
	class RAM : public ROM<T, BE>
	{
		public:
		explicit RAM(size_t a_BankSize, size_t a_BankCount):
			ROM(a_BankSize, a_BankCount)
		{
		}
		
		void Store8(Address a_Address, uint8_t a_Value) override
		{
			GetActiveBankData()[a_Address] = a_Value;
		}
	};
}

#endif