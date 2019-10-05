#include <common/breakpointcondition.hpp>

using namespace Amber;
using namespace Common;

BreakpointConditionType::Enum BreakpointCondition::GetType() const noexcept
{
	return m_Type;
}

uint64_t BreakpointCondition::GetAddress() const noexcept
{
	return m_Data.m_Address;
}

size_t BreakpointCondition::GetEvent() const noexcept
{
	return m_Data.m_Event;
}

void BreakpointCondition::SetType(BreakpointConditionType::Enum a_Type) noexcept
{
	m_Type = a_Type;
}

void BreakpointCondition::SetAddress(uint64_t a_Address) noexcept
{
	m_Data.m_Address = a_Address;
}

void BreakpointCondition::SetEvent(size_t a_Event) noexcept
{
	m_Data.m_Event = a_Event;
}
