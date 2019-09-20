#include <common/bytereader.hpp>

#include <algorithm>
#include <memory>

using namespace Amber;
using namespace Common;

ByteReader::ByteReader(const void* a_Source, size_t a_Size) noexcept:
	m_Source(a_Source),
	m_Size(a_Size)
{
}

const void* ByteReader::GetSource() const noexcept
{
	return m_Source;
}

size_t ByteReader::GetSize() const noexcept
{
	return m_Size;
}

size_t ByteReader::GetPosition() const noexcept
{
	return m_Position;
}

void ByteReader::SetPosition(size_t a_Position) noexcept
{
	m_Position = a_Position;
}

size_t ByteReader::Read(void* a_Destination, size_t a_Size) noexcept
{
	const size_t size = std::min(a_Size, m_Size - m_Position);
	std::memcpy(a_Destination, reinterpret_cast<const uint8_t*>(m_Source) + m_Position, size);
	m_Position += size;

	return size;
}