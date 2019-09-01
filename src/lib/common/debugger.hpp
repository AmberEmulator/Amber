#ifndef H_AMBER_COMMON_DEBUGGER
#define H_AMBER_COMMON_DEBUGGER

#include <common/api.hpp>

#include <set>

namespace Amber::Common
{
	class COMMON_API Debugger
	{
		public:
		virtual ~Debugger() noexcept = 0;

		virtual uint64_t GetMaximumAddress() const noexcept;
		virtual size_t GetMaximumAddressWidth() const noexcept;

		virtual bool IsValidAddress(uint64_t a_Address) const noexcept = 0;
		virtual std::string GetAddressName(uint64_t a_Address) const;
		virtual uint64_t GetInstructionSize(uint64_t a_Address) const = 0;
		virtual std::string GetInstructionName(uint64_t a_Address) const = 0;

		virtual uint8_t Load8(uint64_t a_Address) const = 0;

		virtual bool HasBreakpoint(uint64_t a_Address) const noexcept;
		virtual void SetBreakpoint(uint64_t a_Address, bool a_Enabled);
		virtual std::set<uint64_t> GetBreakpoints() const;

		virtual bool Run() = 0;
		virtual void Step() = 0;
		virtual void Reset() = 0;
	};
}

#endif