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

		private:
		void* m_Destination;
		size_t m_Size;
		size_t m_Position = 0;
	};
}

#endif