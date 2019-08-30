#ifndef H_AMBER_COMMON_RAM
#define H_AMBER_COMMON_RAM

#include <common/rom.h>

namespace Amber::Common
{
	template <typename T, bool BE>
	class RAM : public ROM<T, BE>
	{
		public:
		explicit RAM(size_t a_Size):
			ROM(a_Size)
		{
		}
		
		void Store8(Address a_Address, uint8_t a_Value) override
		{
			GetData()[a_Address] = a_Value;
		}
	};
}

#endif