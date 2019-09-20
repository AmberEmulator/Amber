#include <catch2/catch.hpp>

#include <common/bytewriter.hpp>

using namespace Amber;
using namespace Common;

TEST_CASE("ByteWriter is initialized correctly")
{
	int some_pointer = 345267;
	const size_t some_size = 1238512;

	ByteWriter writer(&some_pointer, some_size);
	REQUIRE(writer.GetDestination() == &some_pointer);
	REQUIRE(writer.GetSize() == some_size);
	REQUIRE(writer.GetPosition() == 0);
}

TEST_CASE("ByteWriter position can be set manually")
{
	ByteWriter writer(nullptr, 1024);

	for (size_t i = 0; i < 1024; ++i)
	{
		writer.SetPosition(i);
		REQUIRE(writer.GetPosition() == i);
		writer.SetPosition(i * i);
		REQUIRE(writer.GetPosition() == i * i);
		writer.SetPosition(i * i * i);
		REQUIRE(writer.GetPosition() == i * i * i);
	}
}

TEST_CASE("ByteWriter can write bytes")
{
	const size_t buffer_size = 1024 * 1024;
	auto buffer = std::make_unique<uint8_t[]>(buffer_size);
	ByteWriter writer(buffer.get(), buffer_size);

	for (size_t i = 0; i < 1024; ++i)
	{
		{
			const uint8_t source = static_cast<uint8_t>(i);
			{
				const size_t start_position = writer.GetPosition();
				const size_t bytes_written = writer.Write(&source, sizeof(source));
				REQUIRE(bytes_written == sizeof(source));
				REQUIRE(writer.GetPosition() == start_position + sizeof(source));
				REQUIRE(std::memcmp(buffer.get() + start_position, &source, sizeof(source)) == 0);
			}
			{
				const size_t start_position = writer.GetPosition();
				const size_t bytes_written = writer.Write(source);
				REQUIRE(bytes_written == sizeof(source));
				REQUIRE(writer.GetPosition() == start_position + sizeof(source));
				REQUIRE(std::memcmp(buffer.get() + start_position, &source, sizeof(source)) == 0);
			}
		}

		{
			const uint16_t source = static_cast<uint16_t>(i * i);
			{
				const size_t start_position = writer.GetPosition();
				const size_t bytes_written = writer.Write(&source, sizeof(source));
				REQUIRE(bytes_written == sizeof(source));
				REQUIRE(writer.GetPosition() == start_position + sizeof(source));
				REQUIRE(std::memcmp(buffer.get() + start_position, &source, sizeof(source)) == 0);
			}
			{
				const size_t start_position = writer.GetPosition();
				const size_t bytes_written = writer.Write(source);
				REQUIRE(bytes_written == sizeof(source));
				REQUIRE(writer.GetPosition() == start_position + sizeof(source));
				REQUIRE(std::memcmp(buffer.get() + start_position, &source, sizeof(source)) == 0);
			}
		}

		{
			const uint32_t source = static_cast<uint32_t>(i * i * i);
			{
				const size_t start_position = writer.GetPosition();
				const size_t bytes_written = writer.Write(&source, sizeof(source));
				REQUIRE(bytes_written == sizeof(source));
				REQUIRE(writer.GetPosition() == start_position + sizeof(source));
				REQUIRE(std::memcmp(buffer.get() + start_position, &source, sizeof(source)) == 0);
			}
			{
				const size_t start_position = writer.GetPosition();
				const size_t bytes_written = writer.Write(source);
				REQUIRE(bytes_written == sizeof(source));
				REQUIRE(writer.GetPosition() == start_position + sizeof(source));
				REQUIRE(std::memcmp(buffer.get() + start_position, &source, sizeof(source)) == 0);
			}
		}

		{
			const uint64_t source = static_cast<uint64_t>(i * i * i * i);
			{
				const size_t start_position = writer.GetPosition();
				const size_t bytes_written = writer.Write(&source, sizeof(source));
				REQUIRE(bytes_written == sizeof(source));
				REQUIRE(writer.GetPosition() == start_position + sizeof(source));
				REQUIRE(std::memcmp(buffer.get() + start_position, &source, sizeof(source)) == 0);
			}
			{
				const size_t start_position = writer.GetPosition();
				const size_t bytes_written = writer.Write(source);
				REQUIRE(bytes_written == sizeof(source));
				REQUIRE(writer.GetPosition() == start_position + sizeof(source));
				REQUIRE(std::memcmp(buffer.get() + start_position, &source, sizeof(source)) == 0);
			}
		}

		{
			const char source[] = "This is a string bla bla bla";
			const size_t start_position = writer.GetPosition();
			const size_t bytes_written = writer.Write(source, sizeof(source));
			REQUIRE(bytes_written == sizeof(source));
			REQUIRE(writer.GetPosition() == start_position + sizeof(source));
			REQUIRE(std::memcmp(buffer.get() + start_position, &source, sizeof(source)) == 0);
		}
	}
}