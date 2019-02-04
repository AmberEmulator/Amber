#include <catch2/catch.hpp>

#include <common/addressmask.h>

#include <algorithm>

using namespace Demu;
using namespace Common;

#define TEST_ADDRESS_TYPES uint8_t, uint16_t, uint32_t, uint64_t
#define TEST_BIT_COUNTS GENERATE(0, 1, 2, 3, 4, 5, 6, 7, 8, 16, 24, 32, 1024, 4096, 16384)
#define TEST_TAGS "[memory]"

TEMPLATE_TEST_CASE("AddressMask is initialized with the correct amount of bits", TEST_TAGS, TEST_ADDRESS_TYPES)
{
	const size_t bit_count = TEST_BIT_COUNTS;
	AddressMask<TestType> address_mask(bit_count);
	
	REQUIRE(address_mask.GetBitCount() == bit_count);
}

TEMPLATE_TEST_CASE("AddressMask bits are initialized to zero", TEST_TAGS, TEST_ADDRESS_TYPES)
{
	const size_t bit_count = TEST_BIT_COUNTS;
	AddressMask<TestType> address_mask(bit_count);

	for (size_t i = 0; i < bit_count; ++i)
	{
		REQUIRE_FALSE(address_mask.GetReadBit(i));
		REQUIRE_FALSE(address_mask.GetWriteBit(i));
	}
}

TEMPLATE_TEST_CASE("AddressMask read and write bits can be set independently", TEST_TAGS, TEST_ADDRESS_TYPES)
{
	const size_t bit_count = TEST_BIT_COUNTS;
	AddressMask<TestType> address_mask(bit_count);

	SECTION("Fill")
	{
		SECTION("Read")
		{
			for (size_t i = 0; i < bit_count; ++i)
			{
				address_mask.SetReadBit(i, true);
			}

			for (size_t i = 0; i < bit_count; ++i)
			{
				REQUIRE(address_mask.GetReadBit(i));
				REQUIRE_FALSE(address_mask.GetWriteBit(i));
			}
		}

		SECTION("Write")
		{
			for (size_t i = 0; i < bit_count; ++i)
			{
				address_mask.SetWriteBit(i, true);
			}

			for (size_t i = 0; i < bit_count; ++i)
			{
				REQUIRE_FALSE(address_mask.GetReadBit(i));
				REQUIRE(address_mask.GetWriteBit(i));
			}
		}

		SECTION("Read/Write")
		{
			for (size_t i = 0; i < bit_count; ++i)
			{
				address_mask.SetReadBit(i, true);
				address_mask.SetWriteBit(i, true);
			}

			for (size_t i = 0; i < bit_count; ++i)
			{
				REQUIRE(address_mask.GetReadBit(i));
				REQUIRE(address_mask.GetWriteBit(i));
			}
		}
	}

	SECTION("Alternate")
	{
		SECTION("Even/Even")
		{
			for (size_t i = 0; i < bit_count; ++i)
			{
				const bool is_even = (i % 2 == 0);

				address_mask.SetReadBit(i, is_even);
				address_mask.SetWriteBit(i, is_even);
			}

			for (size_t i = 0; i < bit_count; ++i)
			{
				const bool is_even = (i % 2 == 0);
				
				REQUIRE(address_mask.GetReadBit(i) == is_even);
				REQUIRE(address_mask.GetWriteBit(i) == is_even);
			}
		}
		SECTION("Even/Odd")
		{
			for (size_t i = 0; i < bit_count; ++i)
			{
				const bool is_even = (i % 2 == 0);

				address_mask.SetReadBit(i, is_even);
				address_mask.SetWriteBit(i, !is_even);
			}

			for (size_t i = 0; i < bit_count; ++i)
			{
				const bool is_even = (i % 2 == 0);

				REQUIRE(address_mask.GetReadBit(i) == is_even);
				REQUIRE(address_mask.GetWriteBit(i) != is_even);
			}
		}
		SECTION("Odd/Even")
		{
			for (size_t i = 0; i < bit_count; ++i)
			{
				const bool is_even = (i % 2 == 0);

				address_mask.SetReadBit(i, !is_even);
				address_mask.SetWriteBit(i, is_even);
			}

			for (size_t i = 0; i < bit_count; ++i)
			{
				const bool is_even = (i % 2 == 0);

				REQUIRE(address_mask.GetReadBit(i) != is_even);
				REQUIRE(address_mask.GetWriteBit(i) == is_even);
			}
		}
		SECTION("Odd/Odd")
		{
			for (size_t i = 0; i < bit_count; ++i)
			{
				const bool is_even = (i % 2 == 0);

				address_mask.SetReadBit(i, !is_even);
				address_mask.SetWriteBit(i, !is_even);
			}

			for (size_t i = 0; i < bit_count; ++i)
			{
				const bool is_even = (i % 2 == 0);

				REQUIRE(address_mask.GetReadBit(i) != is_even);
				REQUIRE(address_mask.GetWriteBit(i) != is_even);
			}
		}
	}
}

TEMPLATE_TEST_CASE("AddressMask bits can be set as ranges", TEST_TAGS, TEST_ADDRESS_TYPES)
{
	const size_t bit_count = TEST_BIT_COUNTS;
	AddressMask<TestType> address_mask(bit_count);

	const size_t offset = std::min<size_t>(GENERATE(0, 10, 100, 1000), bit_count);
	const size_t bits = std::min<size_t>(bit_count - offset, GENERATE(10, 100, 1000));

	SECTION("Read")
	{
		address_mask.SetReadBits(offset, bits, true);

		for (size_t i = 0; i < bit_count; ++i)
		{
			if (i >= offset && i < bits)
			{
				REQUIRE(address_mask.GetReadBit(i));
			}
			else
			{
				REQUIRE_FALSE(address_mask.GetReadBit(i));
			}

			REQUIRE_FALSE(address_mask.GetWriteBit(i));
		}
	}

	SECTION("Write")
	{
		address_mask.SetWriteBits(offset, bits, true);

		for (size_t i = 0; i < bit_count; ++i)
		{
			if (i >= offset && i < bits)
			{
				REQUIRE(address_mask.GetWriteBit(i));
			}
			else
			{
				REQUIRE_FALSE(address_mask.GetWriteBit(i));
			}

			REQUIRE_FALSE(address_mask.GetReadBit(i));
		}
	}

	SECTION("Read/Write")
	{
		address_mask.SetReadBits(offset, bits, true);
		address_mask.SetWriteBits(offset, bits, true);

		for (size_t i = 0; i < bit_count; ++i)
		{
			if (i >= offset && i < bits)
			{
				REQUIRE(address_mask.GetReadBit(i));
				REQUIRE(address_mask.GetWriteBit(i));
			}
			else
			{
				REQUIRE_FALSE(address_mask.GetReadBit(i));
				REQUIRE_FALSE(address_mask.GetWriteBit(i));
			}
		}
	}
}