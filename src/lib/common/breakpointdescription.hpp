#ifndef H_AMBER_COMMON_BREAKPOINTDESCRIPTION
#define H_AMBER_COMMON_BREAKPOINTDESCRIPTION

#include <common/api.hpp>
#include <common/breakpointcondition.hpp>

#include <vector>

namespace Amber::Common
{
	class COMMON_API BreakpointDescription
	{
		public:
		size_t GetConditionCount() const noexcept;
		const BreakpointCondition& GetCondition(size_t a_Index) const noexcept;

		void SetConditionCount(size_t a_Count);
		void SetCondition(size_t a_Index, const BreakpointCondition& a_Condition) noexcept;
		void AddCondition(const BreakpointCondition& a_Condition);

		private:
		std::vector<BreakpointCondition> m_Conditions;
	};
}

#endif