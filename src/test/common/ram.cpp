#include <catch2/catch.hpp>

#include <common/ram.hpp>

#include <algorithm>

using namespace Amber;
using namespace Common;

#define TEST_RAM_TYPES \
	(RAM<uint8_t, false>),\
	(RAM<uint8_t, true>),\
	(RAM<uint16_t, false>),\
	(RAM<uint16_t, true>),\
	(RAM<uint32_t, false>),\
	(RAM<uint32_t, true>),\
	(RAM<uint64_t, false>),\
	(RAM<uint64_t, true>)

#define TEST_ROM_TYPES \
	(ROM<uint8_t, false>),\
	(ROM<uint8_t, true>),\
	(ROM<uint16_t, false>),\
	(ROM<uint16_t, true>),\
	(ROM<uint32_t, false>),\
	(ROM<uint32_t, true>),\
	(ROM<uint64_t, false>),\
	(ROM<uint64_t, true>)

#define TEST_RAM_AND_ROM_TYPES TEST_RAM_TYPES, TEST_ROM_TYPES

#define TEST_ROM_SIZES GENERATE(0, 1, 2, 3, 4, 16, 32, 123, 256)
#define TEST_TAGS "[memory]"

TEMPLATE_TEST_CASE("ROM is initialized with the correct amount of bytes", TEST_TAGS, TEST_RAM_AND_ROM_TYPES)
{
	const size_t rom_size = TEST_ROM_SIZES;
	TestType rom(rom_size);

	REQUIRE(rom.GetSize() == rom_size);
}

TEMPLATE_TEST_CASE("ROM can load bytes", TEST_TAGS, TEST_RAM_AND_ROM_TYPES)
{
	const size_t rom_size = TEST_ROM_SIZES;
	TestType rom(rom_size);

	for (size_t i = 0; i < rom_size; ++i)
	{
		rom.GetData()[i] = static_cast<uint8_t>(i);
	}

	for (size_t i = 0; i < rom_size; ++i)
	{
		const auto address = static_cast<TestType::Address>(i);

		REQUIRE(rom.Load8(address) == static_cast<uint8_t>(i));

		if (TestType::IsBigEndian)
		{
			if (i + 1 < rom_size)
			{
				const uint16_t expected = 
					(static_cast<uint16_t>(i) << 8) |
					(static_cast<uint16_t>(i + 1) << 0);

				REQUIRE(rom.Load16(address) == expected);
			}
			if (i + 3 < rom_size)
			{
				const uint32_t expected =
					(static_cast<uint32_t>(i + 0) << 24) |
					(static_cast<uint32_t>(i + 1) << 16) |
					(static_cast<uint32_t>(i + 2) << 8) |
					(static_cast<uint32_t>(i + 3) << 0);

				REQUIRE(rom.Load32(address) == expected);
			}
			if (i + 7 < rom_size)
			{
				const uint64_t expected = 
					(static_cast<uint64_t>(i + 0) << 56) |
					(static_cast<uint64_t>(i + 1) << 48) |
					(static_cast<uint64_t>(i + 2) << 40) |
					(static_cast<uint64_t>(i + 3) << 32) |
					(static_cast<uint64_t>(i + 4) << 24) |
					(static_cast<uint64_t>(i + 5) << 16) |
					(static_cast<uint64_t>(i + 6) << 8) |
					(static_cast<uint64_t>(i + 7) << 0);

				REQUIRE(rom.Load64(address) == expected);
			}
		}
		else
		{
			if (i + 1 < rom_size)
			{
				const uint16_t expected = 
					(static_cast<uint16_t>(i + 1) << 8) |
					(static_cast<uint16_t>(i + 0) << 0);

				REQUIRE(rom.Load16(address) == expected);
			}
			if (i + 3 < rom_size)
			{
				const uint32_t expected =
					(static_cast<uint32_t>(i + 3) << 24) |
					(static_cast<uint32_t>(i + 2) << 16) |
					(static_cast<uint32_t>(i + 1) << 8) |
					(static_cast<uint32_t>(i + 0) << 0);

				REQUIRE(rom.Load32(address) == expected);
			}
			if (i + 7 < rom_size)
			{
				const uint64_t expected = 
					(static_cast<uint64_t>(i + 7) << 56) |
					(static_cast<uint64_t>(i + 6) << 48) |
					(static_cast<uint64_t>(i + 5) << 40) |
					(static_cast<uint64_t>(i + 4) << 32) |
					(static_cast<uint64_t>(i + 3) << 24) |
					(static_cast<uint64_t>(i + 2) << 16) |
					(static_cast<uint64_t>(i + 1) << 8) |
					(static_cast<uint64_t>(i + 0) << 0);

				REQUIRE(rom.Load64(address) == expected);
			}
		}
	}
}

TEMPLATE_TEST_CASE("RAM can store bytes", TEST_TAGS, TEST_RAM_TYPES)
{
	const size_t ram_size = TEST_ROM_SIZES;
	TestType ram(ram_size);

	for (size_t i = 0; i < ram_size; ++i)
	{
		const auto address = static_cast<TestType::Address>(i);
		ram.Store8(address, static_cast<uint8_t>(i));
	}

	for (size_t i = 0; i < ram_size; ++i)
	{
		const auto address = static_cast<TestType::Address>(i);
		REQUIRE(ram.Load8(address) == static_cast<uint8_t>(i));
	}
}

TEMPLATE_TEST_CASE("ROM cannot store bytes", TEST_TAGS, TEST_ROM_TYPES)
{
	const size_t rom_size = TEST_ROM_SIZES;
	TestType rom(rom_size);

	for (size_t i = 0; i < rom_size; ++i)
	{
		const auto address = static_cast<TestType::Address>(i);
		rom.Store8(address, static_cast<uint8_t>(0xFF));
	}

	for (size_t i = 0; i < rom_size; ++i)
	{
		const auto address = static_cast<TestType::Address>(i);
		REQUIRE(rom.Load8(address) == 0);
	}
}