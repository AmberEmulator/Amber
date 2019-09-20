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

		private:
		const void* m_Source;
		size_t m_Size;
		size_t m_Position = 0;
	};
}

#endif