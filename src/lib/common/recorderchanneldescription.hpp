#ifndef H_AMBER_COMMON_RECORDERCHANNELDESCRIPTION
#define H_AMBER_COMMON_RECORDERCHANNELDESCRIPTION

#include <common/api.hpp>
#include <common/recordermemberdescription.hpp>

#include <optional>
#include <map>
#include <vector>

namespace Amber::Common
{
	class COMMON_API RecorderChannelDescription
	{
		public:
		std::string_view GetName() const noexcept;
		size_t GetSize() const noexcept;

		size_t GetMemberCount() const noexcept;
		std::optional<size_t> FindMember(std::string_view a_Name) const noexcept;
		const RecorderMemberDescription& GetMember(size_t a_Index) const noexcept;

		void SetName(std::string_view a_Name);
		size_t AddMember(const RecorderMemberDescription& a_Description);

		private:
		std::string m_Name;
		std::vector<RecorderMemberDescription> m_Members;
		std::map<std::string, size_t, std::less<>> m_MemberNameMap;
	};
}

#endif