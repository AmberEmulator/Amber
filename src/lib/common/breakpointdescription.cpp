#include <common/breakpointdescription.hpp>

using namespace Amber;
using namespace Common;

size_t BreakpointDescription::GetConditionCount() const noexcept
{
	return m_Conditions.size();
}

const BreakpointCondition& BreakpointDescription::GetCondition(size_t a_Index) const noexcept
{
	return m_Conditions[a_Index];
}

void BreakpointDescription::SetConditionCount(size_t a_Count)
{
	m_Conditions.resize(a_Count);
}

void BreakpointDescription::SetCondition(size_t a_Index, const BreakpointCondition& a_Condition) noexcept
{
	m_Conditions[a_Index] = a_Condition;
}

void BreakpointDescription::AddCondition(const BreakpointCondition& a_Condition)
{
	const size_t index = GetConditionCount();
	SetConditionCount(index + 1);
	SetCondition(index, a_Condition);
}