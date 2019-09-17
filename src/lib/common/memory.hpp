#ifndef H_AMBER_COMMON_MEMORY
#define H_AMBER_COMMON_MEMORY

#include <common/api.hpp>

#include <optional>
#include <type_traits>
#include <unordered_map>

namespace Amber::Common
{
	template <typename T>
	class Memory
	{
		public:
		using Address = T;
		static_assert(std::is_unsigned_v<Address> && !std::is_same_v<Address, bool>, "Address must be an unsigned integer");

		virtual ~Memory() noexcept = 0;

		virtual uint8_t  Load8(Address a_Address) const = 0;
		virtual uint16_t Load16(Address a_Address) const = 0;
		virtual uint32_t Load32(Address a_Address) const = 0;
		virtual uint64_t Load64(Address a_Address) const = 0;

		virtual void Store8(Address a_Address, uint8_t  a_Value) = 0;
		virtual void Store16(Address a_Address, uint16_t a_Value) = 0;
		virtual void Store32(Address a_Address, uint32_t a_Value) = 0;
		virtual void Store64(Address a_Address, uint64_t a_Value) = 0;

		template <typename U>
		U Load(Address a_Address)
		{
			static_assert(std::is_unsigned_v<U> && !std::is_same_v<Address, bool>);

			if constexpr (std::is_same_v<U, uint8_t>)
			{
				return Load8(a_Address);
			}
			else if constexpr (std::is_same_v<U, uint16_t>)
			{
				return Load16(a_Address);
			}
			else if constexpr (std::is_same_v<U, uint32_t>)
			{
				return Load32(a_Address);
			}
			else if constexpr (std::is_same_v<U, uint64_t>)
			{
				return Load64(a_Address);
			}
		}

		template <typename U>
		void Store(Address a_Address, U a_Value)
		{
			static_assert(std::is_unsigned_v<U> && !std::is_same_v<Address, bool>);

			if constexpr (std::is_same_v<U, uint8_t>)
			{
				Store8(a_Address, a_Value);
			}
			else if constexpr (std::is_same_v<U, uint16_t>)
			{
				Store16(a_Address, a_Value);
			}
			else if constexpr (std::is_same_v<U, uint32_t>)
			{
				Store32(a_Address, a_Value);
			}
			else if constexpr (std::is_same_v<U, uint64_t>)
			{
				Store64(a_Address, a_Value);
			}
		}

		virtual uint64_t GetPhysicalAddress(Address a_Address) const
		{
			return static_cast<uint64_t>(a_Address);
		}

		void Replace8(Address a_Address, uint8_t a_Value)
		{
			const uint64_t physical_address = GetPhysicalAddress(a_Address);
			const auto it = m_ReplacedBytes.find(physical_address);
			if (it == m_ReplacedBytes.end())
			{
				const uint8_t original_value = Load8(a_Address);
				m_ReplacedBytes.emplace(physical_address, original_value);
			}

			Store8(a_Address, a_Value);
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
			const uint16_t byte0 = Load8(a_Address);
			const uint16_t byte1 = Load8(a_Address + 1);

			if constexpr (IsBigEndian)
			{
				return (byte0 << 8) | byte1;
			}
			else
			{
				return (byte1 << 8) | byte0;
			}
		}

		virtual uint32_t Load32(Address a_Address) const override
		{
			const uint32_t word0 = Load16(a_Address);
			const uint32_t word1 = Load16(a_Address + 2);

			if constexpr (IsBigEndian)
			{
				return (word0 << 16) | word1;
			}
			else
			{
				return (word1 << 16) | word0;
			}
		}

		virtual uint64_t Load64(Address a_Address) const override
		{
			const uint64_t dword0 = Load32(a_Address);
			const uint64_t dword1 = Load32(a_Address + 4);

			if constexpr (IsBigEndian)
			{
				return (dword0 << 32) | dword1;
			}
			else
			{
				return (dword1 << 32) | dword0;
			}
		}

		virtual void Store16(Address a_Address, uint16_t a_Value) override
		{
			const uint8_t byte0 = a_Value & 0xFF;
			const uint8_t byte1 = (a_Value >> 8) & 0xFF;

			if constexpr (IsBigEndian)
			{
				Store8(a_Address, byte1);
				Store8(a_Address + 1, byte0);
			}
			else
			{
				Store8(a_Address, byte0);
				Store8(a_Address + 1, byte1);
			}
		}

		virtual void Store32(Address a_Address, uint32_t a_Value) override
		{
			const uint16_t word0 = a_Value & 0xFFFF;
			const uint16_t word1 = (a_Value >> 16) & 0xFFFF;

			if constexpr (IsBigEndian)
			{
				Store16(a_Address, word1);
				Store16(a_Address + 2, word0);
			}
			else
			{
				Store16(a_Address, word0);
				Store16(a_Address + 2, word1);
			}
		}

		virtual void Store64(Address a_Address, uint64_t a_Value) override
		{
			const uint32_t dword0 = a_Value & 0xFFFFFFFF;
			const uint32_t dword1 = (a_Value >> 32) & 0xFFFFFFFF;

			if constexpr (IsBigEndian)
			{
				Store32(a_Address, dword1);
				Store32(a_Address + 4, dword0);
			}
			else
			{
				Store32(a_Address, dword0);
				Store32(a_Address + 4, dword1);
			}
		}
	};
}

#endif