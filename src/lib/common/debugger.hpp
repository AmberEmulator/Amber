#ifndef H_AMBER_COMMON_DEBUGGER
#define H_AMBER_COMMON_DEBUGGER

#include <common/api.hpp>
#include <common/breakpointdescription.hpp>

#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

namespace Amber::Common
{
	using Breakpoint = struct {} *;

	class COMMON_API Debugger
	{
		public:
		using Address = uint64_t;

		virtual ~Debugger() noexcept = 0;

		virtual uint64_t GetMaximumAddress() const noexcept;
		virtual size_t GetMaximumAddressWidth() const noexcept;

		virtual bool IsValidAddress(uint64_t a_Address) const noexcept = 0;
		virtual std::string GetAddressName(uint64_t a_Address) const;
		virtual uint64_t GetInstructionSize(uint64_t a_Address) const = 0;
		virtual std::string GetInstructionName(uint64_t a_Address) const = 0;

		virtual uint8_t Load8(uint64_t a_Address) const = 0;

		// Events
		virtual size_t GetEventCount() const noexcept;
		virtual std::string GetEventName(size_t a_Event) const;

		// Breakpoints
		Breakpoint CreateBreakpoint(const BreakpointDescription& a_Description);
		void DestroyBreakpoint(Breakpoint a_Breakpoint) noexcept;

		size_t GetBreakpointCount() const noexcept;
		Breakpoint GetBreakpoint(size_t a_Index) const noexcept;
		std::optional<size_t> GetBreakpointIndex(Breakpoint a_Breakpoint) const noexcept;
		const BreakpointDescription& GetBreakpointDescription(Breakpoint a_Breakpoint) const noexcept;
		std::vector<Breakpoint> GetExecutionBreakpoints(Address a_Address) const noexcept;

		// Execution
		virtual bool Run() = 0;
		virtual bool Step() = 0;
		virtual bool Microstep();
		virtual bool Reset() = 0;

		protected:
		virtual void OnBreakpointCreate(Breakpoint a_Breakpoint);
		virtual void OnBreakpointDestroy(Breakpoint a_Breakpoint) noexcept;

		private:
		struct BreakpointInfo
		{
			BreakpointDescription m_Description;
		};

		std::vector<std::unique_ptr<BreakpointInfo>> m_Breakpoints;
		std::unordered_map<Breakpoint, size_t> m_BreakpointIndexMap;
		std::unordered_multimap<uint64_t, Breakpoint> m_ExecutionBreakpointMap;
	};
}

#endif