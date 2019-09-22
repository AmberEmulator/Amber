#ifndef H_AMBER_COMMON_BYTEREADER
#define H_AMBER_COMMON_BYTEREADER

#include <common/api.hpp>

namespace Amber::Common
{
	class COMMON_API ByteReader
	{
		public:
		ByteReader(const void* a_Source, size_t a_Size) noexcept;

		const void* GetSource() const noexcept;
		size_t GetSize() const noexcept;
		size_t GetPosition() const noexcept;

		void SetPosition(size_t a_Position) noexcept;

		size_t Read(void* a_Destination, size_t a_Size) noexcept;

		template <typename T>
		size_t Read(T& a_Destination) noexcept
		{
			return Read(&a_Destination, sizeof(T));
		}

		template <typename T>
		size_t ReadVLQ(T& a_Quantity) noexcept
		{
			a_Quantity = 0;
			size_t bytes_read = 0;

			for (;;)
			{
				uint8_t byte = 0;
				bytes_read += Read(byte);

				a_Quantity <<= 7;
				a_Quantity |= byte & 0b0111'1111;

				if (byte & 0b1000'0000)
				{
					break;
				}
			}

			return bytes_read;
		}

		private:
		const void* m_Source;
		size_t m_Size;
		size_t m_Position = 0;
	};
}

#endif