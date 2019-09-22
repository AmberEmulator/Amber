#include <common/bytewriter.hpp>

#include <algorithm>
#include <memory>

using namespace Amber;
using namespace Common;

ByteWriter::ByteWriter(void* a_Destination, size_t a_Size) noexcept:
	m_Destination(a_Destination),
	m_Size(a_Size)
{
}

void* ByteWriter::GetDestination() const noexcept
{
	return m_Destination;
}

size_t ByteWriter::GetSize() const noexcept
{
	return m_Size;
}

size_t ByteWriter::GetPosition() const noexcept
{
	return m_Position;
}

void ByteWriter::SetPosition(size_t a_Position) noexcept
{
	m_Position = a_Position;
}

size_t ByteWriter::Write(const void* a_Source, size_t a_Size) noexcept
{
	const size_t size = std::min(a_Size, m_Size - m_Position);
	std::memcpy(reinterpret_cast<uint8_t*>(m_Destination) + m_Position, a_Source, size);
	m_Position += size;

	return size;
}