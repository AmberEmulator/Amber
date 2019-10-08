#ifndef H_AMBER_COMMON_BREAKPOINTCONDITIONTYPE
#define H_AMBER_COMMON_BREAKPOINTCONDITIONTYPE

#include <common/api.hpp>

#include <array>
#include <optional>

namespace Amber::Common
{
	namespace BreakpointConditionType
	{
		enum Enum
		{
			Execution,
			Event
		};

		constexpr std::array<BreakpointConditionType::Enum, 2> Enums = { BreakpointConditionType::Execution, BreakpointConditionType::Event };

		constexpr std::optional<std::string_view> ToString(BreakpointConditionType::Enum a_Value) noexcept
		{
			switch (a_Value)
			{
				case BreakpointConditionType::Execution:
				return "Execution";

				case BreakpointConditionType::Event:
				return "Event";

				default:
				return {};
			}
		}
	};
}

#endif