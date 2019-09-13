#ifndef H_AMBER_COMMON_RAM
#define H_AMBER_COMMON_RAM

#include <common/rom.hpp>

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

	template <bool BE> using RAM8  = RAM<uint8_t, BE>;
	template <bool BE> using RAM16 = RAM<uint16_t, BE>;
	template <bool BE> using RAM32 = RAM<uint32_t, BE>;
	template <bool BE> using RAM64 = RAM<uint64_t, BE>;
}

#endif