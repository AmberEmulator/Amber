#include <common/debugger.hpp>

#include <limits>

using namespace Amber;
using namespace Common;

Debugger::~Debugger() noexcept = default;

uint64_t Debugger::GetMaximumAddress() const noexcept
{
	return std::numeric_limits<uint64_t>::max();
}

size_t Debugger::GetMaximumAddressWidth() const noexcept
{
	// TODO: portable
	const uint64_t maximum_address = GetMaximumAddress();
	unsigned long result;
	_BitScanReverse64(&result, maximum_address);
	return static_cast<size_t>((result + 4) / 4);
}

std::string Debugger::GetAddressName(uint64_t a_Address) const
{
	// Hex character lookup table
	constexpr char hex_characters[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

	// Calculate address width
	const size_t address_Width = GetMaximumAddressWidth();

	// Create string
	std::string name;
	name.resize(address_Width);

	// Find all characters by shifting and masking the address
	for (size_t i = 0; i < address_Width; ++i)
	{
		name[(address_Width - 1) - i] = hex_characters[(a_Address >> (i * 4)) & 0xF];
	}

	return name;
}

bool Debugger::Microstep()
{
	Step();
	return true;
}

size_t Debugger::GetBreakpointCount() const noexcept
{
	return m_Breakpoints.size();
}

Breakpoint Debugger::GetBreakpoint(size_t a_Index) const noexcept
{
	return reinterpret_cast<Breakpoint>(m_Breakpoints[a_Index].get());
}

std::optional<size_t> Debugger::GetBreakpointIndex(Breakpoint a_Breakpoint) const noexcept
{
	const auto it = m_BreakpointIndexMap.find(a_Breakpoint);
	if (it != m_BreakpointIndexMap.end())
	{
		return it->second;
	}
	else
	{
		return {};
	}
}

Breakpoint Debugger::CreateBreakpoint(const BreakpointDescription& a_Description)
{
	// Initialize breakpoint
	auto breakpoint_info = std::make_unique<BreakpointInfo>();
	breakpoint_info->m_Description = a_Description;
	const auto breakpoint = reinterpret_cast<Breakpoint>(breakpoint_info.get());

	// Add breakpoint to the containers
	// TODO: Strong exception guarantee
	const size_t breakpoint_index = m_Breakpoints.size();

	m_BreakpointIndexMap.emplace(breakpoint, breakpoint_index);
	for (size_t condition_index = 0; condition_index < breakpoint_info->m_Description.GetConditionCount(); ++condition_index)
	{
		auto& condition = breakpoint_info->m_Description.GetCondition(condition_index);
		if (condition.GetType() == BreakpointConditionType::Execution)
		{
			m_ExecutionBreakpointMap.emplace(condition.GetAddress(), breakpoint);
		}
	}

	m_Breakpoints.emplace_back(std::move(breakpoint_info));

	// Invoke create callback
	OnBreakpointCreate(breakpoint);

	// Return result
	return breakpoint;
}

void Debugger::DestroyBreakpoint(Breakpoint a_Breakpoint) noexcept
{
	// Ignore null
	if (a_Breakpoint == nullptr)
	{
		return;
	}

	// Terminate on invalid breakpoints
	const auto breakpoint_index_it = m_BreakpointIndexMap.find(a_Breakpoint);
	if (breakpoint_index_it == m_BreakpointIndexMap.end())
	{
		std::terminate();
	}

	// Invoke destroy callback
	OnBreakpointDestroy(a_Breakpoint);

	// Get breakpoint information
	auto& breakpoint_info = *reinterpret_cast<BreakpointInfo*>(a_Breakpoint);
	const size_t breakpoint_index = breakpoint_index_it->second;

	// Remove from execution map
	for (size_t condition_index = 0; condition_index < breakpoint_info.m_Description.GetConditionCount(); ++condition_index)
	{
		auto& condition = breakpoint_info.m_Description.GetCondition(condition_index);
		if (condition.GetType() == BreakpointConditionType::Execution)
		{
			const auto execution_range = m_ExecutionBreakpointMap.equal_range(condition.GetAddress());
			for (auto execution_it = execution_range.first; execution_it != execution_range.second; ++execution_it)
			{
				if (execution_it->second == a_Breakpoint)
				{
					m_ExecutionBreakpointMap.erase(execution_it);
					break;
				}
			}
		}
	}

	// Remove from index map
	for (size_t i = breakpoint_index + 1; i < m_Breakpoints.size(); ++i)
	{
		const auto next_breakpoint = reinterpret_cast<Breakpoint>(m_Breakpoints[i].get());
		m_BreakpointIndexMap.find(next_breakpoint)->second -= 1;
	}
	m_BreakpointIndexMap.erase(breakpoint_index_it);

	// Delete breakpoint info
	m_Breakpoints.erase(m_Breakpoints.begin() + breakpoint_index);
}

const BreakpointDescription& Debugger::GetBreakpointDescription(Breakpoint a_Breakpoint) const noexcept
{
	auto& breakpoint_info = *reinterpret_cast<BreakpointInfo*>(a_Breakpoint);
	return breakpoint_info.m_Description;
}

std::vector<Breakpoint> Debugger::GetExecutionBreakpoints(Address a_Address) const noexcept
{
	std::vector<Breakpoint> execution_breakpoints;
	const auto execution_range = m_ExecutionBreakpointMap.equal_range(a_Address);
	for (auto execution_it = execution_range.first; execution_it != execution_range.second; ++execution_it)
	{
		execution_breakpoints.push_back(execution_it->second);
	}

	return execution_breakpoints;
}

void Debugger::OnBreakpointCreate(Breakpoint a_Breakpoint)
{
}

void Debugger::OnBreakpointDestroy(Breakpoint a_Breakpoint) noexcept
{
}