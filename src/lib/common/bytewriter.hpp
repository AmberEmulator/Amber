#ifndef H_AMBER_COMMON_BYTEWRITER
#define H_AMBER_COMMON_BYTEWRITER

#include <common/api.hpp>

namespace Amber::Common
{
	class COMMON_API ByteWriter
	{
		public:
		ByteWriter(void* a_Destination, size_t a_Size) noexcept;

		void* GetDestination() const noexcept;
		size_t GetSize() const noexcept;
		size_t GetPosition() const noexcept;

		void SetPosition(size_t a_Position) noexcept;

		size_t Write(const void* a_Source, size_t a_Size) noexcept;

		template <typename T>
		size_t Write(const T& a_Source) noexcept
		{
			return Write(&a_Source, sizeof(T));
		}

		template <typename T>
		size_t WriteVLQ(T a_Quantity) noexcept
		{
			size_t bytes_written = 0;
			do
			{
				uint8_t byte = a_Quantity & 0b0111'1111;
				a_Quantity >>= 7;
				if (a_Quantity != 0)
				{
					byte |= 0b1000'0000;
				}

				const uint8_t result = Write(byte);
				if (result == 0)
				{
					return 0;
				}

				bytes_written += result;
			}
			while (a_Quantity != 0);

			m_Position += bytes_written;
			return bytes_written;
		}

		private:
		void* m_Destination;
		size_t m_Size;
		size_t m_Position = 0;
	};
}

#endif