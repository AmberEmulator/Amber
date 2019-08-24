#ifndef H_DEMU_COMMON_MEMORY
#define H_DEMU_COMMON_MEMORY

#include <common/addressmask.h>

#include <optional>
#include <unordered_map>

namespace Demu::Common
{
	template <typename T>
	class Memory
	{
		public:
		using Address = T;
		static_assert(std::is_unsigned_v<Address> && !std::is_same_v<Address, bool>, "Address must be an unsigned integer");

		virtual ~Memory() noexcept = 0;

		virtual AddressMask<Address> GetAddressMask() const = 0;

		virtual uint8_t  Load8(Address a_Address) const = 0;
		virtual uint16_t Load16(Address a_Address) const = 0;
		virtual uint32_t Load32(Address a_Address) const = 0;
		virtual uint64_t Load64(Address a_Address) const = 0;

		virtual void Store8(Address a_Address, uint8_t  a_Value) = 0;
		virtual void Store16(Address a_Address, uint16_t a_Value) = 0;
		virtual void Store32(Address a_Address, uint32_t a_Value) = 0;
		virtual void Store64(Address a_Address, uint64_t a_Value) = 0;

		virtual uint64_t GetPhysicalAddress(Address a_Address) const
		{
			return static_cast<uint64_t>(a_Address);
		}

		void Replace8(Address a_Address, uint8_t a_Value)
		{
			const uint8_t original_value = Load8(a_Address);
			Store8(a_Address, a_Value);

			const uint64_t physical_address = GetPhysicalAddress(a_Address);
			m_ReplacedBytes.insert_or_assign(physical_address, original_value);
		}

		void Restore8(Address a_Address)
		{
			const uint64_t physical_address = GetPhysicalAddress(a_Address);
			const auto it = m_ReplacedBytes.find(physical_address);
			if (it == m_ReplacedBytes.end())
			{
				return;
			}

			const uint8_t original_value = it->second;
			Store8(a_Address, original_value);
			m_ReplacedBytes.erase(it);
		}

		std::optional<uint8_t> GetReplaced8(Address a_Address)
		{
			const uint64_t physical_address = GetPhysicalAddress(a_Address);
			const auto it = m_ReplacedBytes.find(physical_address);
			if (it == m_ReplacedBytes.end())
			{
				return {};
			}

			const uint8_t original_value = it->second;
			return original_value;
		}

		private:
		std::unordered_map<uint64_t, uint8_t> m_ReplacedBytes;
	};

	template <typename T>
	Memory<T>::~Memory() noexcept = default;

	using Memory8 = Memory<uint8_t>;
	using Memory16 = Memory<uint16_t>;
	using Memory32 = Memory<uint32_t>;
	using Memory64 = Memory<uint64_t>;

	template <typename T, bool BE>
	class MemoryHelper : public Memory<T>
	{
		public:
		constexpr static bool IsBigEndian = BE;

		virtual uint16_t Load16(Address a_Address) const override
		{
			if constexpr (IsBigEndian)
			{
				return (static_cast<uint16_t>(Load8(a_Address)) << 8) | Load8(a_Address + 1);
			}
			else
			{
				return (static_cast<uint16_t>(Load8(a_Address + 1)) << 8) | Load8(a_Address);
			}
		}

		virtual uint32_t Load32(Address a_Address) const override
		{
			if constexpr (IsBigEndian)
			{
				return (static_cast<uint32_t>(Load16(a_Address)) << 16) | Load16(a_Address + 2);
			}
			else
			{
				return (static_cast<uint32_t>(Load16(a_Address + 2)) << 16) | Load16(a_Address);
			}
		}

		virtual uint64_t Load64(Address a_Address) const override
		{
			if constexpr (IsBigEndian)
			{
				return (static_cast<uint64_t>(Load32(a_Address)) << 32) | Load32(a_Address + 4);
			}
			else
			{
				return (static_cast<uint64_t>(Load32(a_Address + 4)) << 32) | Load32(a_Address);
			}
		}

		virtual void Store16(Address a_Address, uint16_t a_Value) override
		{
			if constexpr (IsBigEndian)
			{
				Store8(a_Address + 1, (a_Value >> 8) & 0xFF);
				Store8(a_Address, a_Value & 0xFF);
			}
			else
			{
				Store8(a_Address, (a_Value >> 8) & 0xFF);
				Store8(a_Address + 1, a_Value & 0xFF);
			}
		}

		virtual void Store32(Address a_Address, uint32_t a_Value) override
		{
			if constexpr (IsBigEndian)
			{
				Store16(a_Address + 2, (a_Value >> 16) & 0xFFFF);
				Store16(a_Address, a_Value & 0xFFFF);
			}
			else
			{
				Store16(a_Address, (a_Value >> 16) & 0xFFFF);
				Store16(a_Address + 2, a_Value & 0xFFFF);
			}
		}

		virtual void Store64(Address a_Address, uint64_t a_Value) override
		{
			if constexpr (IsBigEndian)
			{
				Store32(a_Address + 4, (a_Value >> 32) & 0xFFFFFFFF);
				Store32(a_Address, a_Value & 0xFFFFFFFF);
			}
			else
			{
				Store32(a_Address, (a_Value >> 32) & 0xFFFFFFFF);
				Store32(a_Address + 4, a_Value & 0xFFFFFFFF);
			}
		}
	};
}

#endif