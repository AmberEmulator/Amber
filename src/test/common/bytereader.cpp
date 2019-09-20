#include <catch2/catch.hpp>

#include <common/bytereader.hpp>
#include <common/bytewriter.hpp>

using namespace Amber;
using namespace Common;

TEST_CASE("ByteReader is initialized correctly")
{
	const int some_pointer = 345267;
	const size_t some_size = 1238512;

	ByteReader reader(&some_pointer, some_size);
	REQUIRE(reader.GetSource() == &some_pointer);
	REQUIRE(reader.GetSize() == some_size);
	REQUIRE(reader.GetPosition() == 0);
}

TEST_CASE("ByteReader position can be set manually")
{
	ByteReader reader(nullptr, 1024);

	for (size_t i = 0; i < 1024; ++i)
	{
		reader.SetPosition(i);
		REQUIRE(reader.GetPosition() == i);
		reader.SetPosition(i * i);
		REQUIRE(reader.GetPosition() == i * i);
		reader.SetPosition(i * i * i);
		REQUIRE(reader.GetPosition() == i * i * i);
	}
}

TEST_CASE("ByteWriter can read bytes")
{
	const size_t buffer_size = 1024 * 1024;
	auto buffer = std::make_unique<uint8_t[]>(buffer_size);
	ByteReader reader(buffer.get(), buffer_size);
	ByteWriter writer(buffer.get(), buffer_size);

	for (size_t i = 0; i < 1024; ++i)
	{
		{
			const uint8_t source = static_cast<uint8_t>(i);
			uint8_t destination;
			{
				const size_t bytes_written = writer.Write(&source, sizeof(source));
				const size_t bytes_read = reader.Read(&destination, sizeof(destination));
				REQUIRE(bytes_read == bytes_written);
				REQUIRE(reader.GetPosition() == writer.GetPosition());
				REQUIRE(source == destination);
			}
			{
				const size_t bytes_written = writer.Write(source);
				const size_t bytes_read = reader.Read(destination);
				REQUIRE(bytes_read == bytes_written);
				REQUIRE(reader.GetPosition() == writer.GetPosition());
				REQUIRE(source == destination);
			}
		}

		{
			const uint16_t source = static_cast<uint16_t>(i * i);
			uint16_t destination;
			{
				const size_t bytes_written = writer.Write(&source, sizeof(source));
				const size_t bytes_read = reader.Read(&destination, sizeof(destination));
				REQUIRE(bytes_read == bytes_written);
				REQUIRE(reader.GetPosition() == writer.GetPosition());
				REQUIRE(source == destination);
			}
			{
				const size_t bytes_written = writer.Write(source);
				const size_t bytes_read = reader.Read(destination);
				REQUIRE(bytes_read == bytes_written);
				REQUIRE(reader.GetPosition() == writer.GetPosition());
				REQUIRE(source == destination);
			}
		}

		{
			const uint32_t source = static_cast<uint32_t>(i * i * i);
			uint32_t destination;
			{
				const size_t bytes_written = writer.Write(&source, sizeof(source));
				const size_t bytes_read = reader.Read(&destination, sizeof(destination));
				REQUIRE(bytes_read == bytes_written);
				REQUIRE(reader.GetPosition() == writer.GetPosition());
				REQUIRE(source == destination);
			}
			{
				const size_t bytes_written = writer.Write(source);
				const size_t bytes_read = reader.Read(destination);
				REQUIRE(bytes_read == bytes_written);
				REQUIRE(reader.GetPosition() == writer.GetPosition());
				REQUIRE(source == destination);
			}
		}

		{
			const uint64_t source = static_cast<uint64_t>(i * i * i * i);
			uint64_t destination;
			{
				const size_t bytes_written = writer.Write(&source, sizeof(source));
				const size_t bytes_read = reader.Read(&destination, sizeof(destination));
				REQUIRE(bytes_read == bytes_written);
				REQUIRE(reader.GetPosition() == writer.GetPosition());
				REQUIRE(source == destination);
			}
			{
				const size_t bytes_written = writer.Write(source);
				const size_t bytes_read = reader.Read(destination);
				REQUIRE(bytes_read == bytes_written);
				REQUIRE(reader.GetPosition() == writer.GetPosition());
				REQUIRE(source == destination);
			}
		}

		{
			const char source[] = "This is a string bla bla bla";
			char destination[sizeof(source)];

			const size_t bytes_written = writer.Write(source, sizeof(source));
			const size_t bytes_read = reader.Read(destination, sizeof(destination));
			REQUIRE(bytes_read == bytes_written);
			REQUIRE(reader.GetPosition() == writer.GetPosition());
			REQUIRE(std::strcmp(source, destination) == 0);
		}
	}
}