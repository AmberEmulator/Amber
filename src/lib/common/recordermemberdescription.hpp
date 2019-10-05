#ifndef H_AMBER_COMMON_RECORDERMEMBERDESCRIPTION
#define H_AMBER_COMMON_RECORDERMEMBERDESCRIPTION

#include <common/api.hpp>
#include <common/recordermembertype.hpp>

namespace Amber::Common
{
	class COMMON_API RecorderMemberDescription
	{
		public:
		std::string_view GetName() const noexcept;
		size_t GetSize() const noexcept;
		RecorderMemberType::Enum GetType() const noexcept;

		void SetName(std::string_view a_Name) noexcept;
		void SetSize(size_t a_Size) noexcept;
		void SetType(RecorderMemberType::Enum a_Type) noexcept;

		private:
		std::string m_Name;
		size_t m_Size = 0;
		RecorderMemberType::Enum m_Type = RecorderMemberType::Raw;
	};
}

#endif