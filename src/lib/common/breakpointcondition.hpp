#ifndef H_AMBER_COMMON_BREAKPOINTCONDITION
#define H_AMBER_COMMON_BREAKPOINTCONDITION

#include <common/api.hpp>
#include <common/breakpointconditiontype.hpp>

namespace Amber::Common
{
	class COMMON_API BreakpointCondition
	{
		public:
		BreakpointConditionType::Enum GetType() const noexcept;
		uint64_t GetAddress() const noexcept;
		size_t GetEvent() const noexcept;

		void SetType(BreakpointConditionType::Enum a_Type) noexcept;
		void SetAddress(uint64_t a_Address) noexcept;
		void SetEvent(size_t a_Event) noexcept;

		private:
		BreakpointConditionType::Enum m_Type = BreakpointConditionType::Execution;
		union
		{
			uint64_t m_Address = 0;
			size_t m_Event;
		} m_Data;
	};
}

#endif