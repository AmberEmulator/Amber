#ifndef H_AMBER_COMMON_MEMORYMAPPING
#define H_AMBER_COMMON_MEMORYMAPPING

#include <common/api.hpp>

namespace Amber::Common
{
	namespace MemoryAccess
	{
		enum Enum
		{
			Read = 0x01,
			Write = 0x02
		};
	}

	template <typename T>
	class MemoryMapping
	{
		public:
		using Address = T;

		Address GetStart() const noexcept
		{
			return m_Start;
		}

		Address GetSize() const noexcept
		{
			return m_Size;
		}

		Address GetOffset() const noexcept
		{
			return m_Offset;
		}

		MemoryAccess::Enum GetAccess() const noexcept
		{
			return m_Access;
		}

		void SetStart(Address a_Start) noexcept
		{
			m_Start = a_Start;
		}

		void SetSize(Address a_Size) noexcept
		{
			m_Size = a_Size;
		}

		void SetOffset(Address a_Offset) noexcept
		{
			m_Offset = a_Offset;
		}

		void SetAccess(MemoryAccess::Enum a_Access)
		{
			m_Access = a_Access;
		}

		private:
		Address m_Start = 0;
		Address m_Size = 0;
		Address m_Offset = 0;
		MemoryAccess::Enum m_Access = Read | Write;
	};
}

#endif