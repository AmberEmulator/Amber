#ifndef H_AMBER_COMMON_REGISTER
#define H_AMBER_COMMON_REGISTER

#include <gameboy/api.hpp>

#include <memory>
#include <type_traits>

namespace Amber::Common
{
	namespace Internal
	{
		// Generic operations
		template <typename T>
		class RegisterBase
		{
			public:
			template <typename U>
			U Load(size_t a_Index) const noexcept
			{
				// TODO: big endian support
				constexpr size_t start = sizeof(T) - sizeof(U);

				U value;
				std::memcpy(&value, (m_Data + start) - sizeof(U) * a_Index, sizeof(U));
				return value;
			}

			template <typename U>
			void Store(size_t a_Index, U a_Value) noexcept
			{
				// TODO: big endian support
				constexpr size_t start = sizeof(T) - sizeof(U);

				std::memcpy((m_Data + start) - sizeof(U) * a_Index, &a_Value, sizeof(U));
			}

			private:
			uint8_t m_Data[sizeof(T)] = {};
		};

		// 64 bit operations
		template <typename T, bool NotIndexed = std::is_same_v<T, uint64_t>>
		class Register64 : public RegisterBase<T>
		{
			public:
			uint64_t Load64(size_t a_Index) const noexcept
			{
				return Load<uint64_t>(a_Index);
			}

			void Store64(size_t a_Index, uint64_t a_Value) noexcept
			{
				Store<uint64_t>(a_Index, a_Value);
			}
		};

		template <typename T>
		class Register64<T, true> : public Register64<T, false>
		{
			using Register64<T, false>::Load64;
			uint64_t Load64() const noexcept
			{
				return RegisterIndexed64::Load64(0);
			}

			using Register64<T, false>::Store64;
			void Store64(uint64_t a_Value) noexcept
			{
				return RegisterIndexed64::Store64(0, a_Value);
			}
		};

		// 32 bit operations
		template <typename T, bool NotIndexed = std::is_same_v<T, uint32_t>>
		class Register32 : public std::conditional_t<std::is_same_v<T, uint32_t>, RegisterBase<T>, Register64<T>>
		{
			public:
			uint32_t Load32(size_t a_Index) const noexcept
			{
				return Load<uint32_t>(a_Index);
			}

			void Store32(size_t a_Index, uint32_t a_Value) noexcept
			{
				Store<uint32_t>(a_Index, a_Value);
			}
		};

		template <typename T>
		class Register32<T, true> : public Register32<T, false>
		{
			using Register32<T, false>::Load32;
			uint32_t Load32() const noexcept
			{
				return RegisterIndexed32::Load32(0);
			}

			using Register32<T, false>::Store32;
			void Store32(uint32_t a_Value) noexcept
			{
				return RegisterIndexed32::Store32(0, a_Value);
			}
		};

		// 16 bit operations
		template <typename T, bool NotIndexed = std::is_same_v<T, uint16_t>>
		class Register16 : public std::conditional_t<std::is_same_v<T, uint16_t>, RegisterBase<T>, Register32<T>>
		{
			public:
			uint16_t Load16(size_t a_Index) const noexcept
			{
				return Load<uint16_t>(a_Index);
			}

			void Store16(size_t a_Index, uint16_t a_Value) noexcept
			{
				Store<uint16_t>(a_Index, a_Value);
			}
		};

		template <typename T>
		class Register16<T, true> : public Register16<T, false>
		{
			using Register16<T, false>::Load16;
			uint16_t Load16() const noexcept
			{
				return RegisterIndexed16::Load16(0);
			}

			using Register16<T, false>::Store16;
			void Store16(uint16_t a_Value) noexcept
			{
				return RegisterIndexed16::Store16(0, a_Value);
			}
		};

		// 8 bit operations
		template <typename T, bool NotIndexed = std::is_same_v<T, uint8_t>>
		class Register8 : public std::conditional_t<std::is_same_v<T, uint8_t>, RegisterBase<T>, Register16<T>>
		{
			public:
			uint8_t Load8(size_t a_Index) const noexcept
			{
				return Load<uint8_t>(a_Index);
			}

			void Store8(size_t a_Index, uint8_t a_Value) noexcept
			{
				Store<uint8_t>(a_Index, a_Value);
			}
		};

		template <typename T>
		class Register8<T, true> : public Register8<T, false>
		{
			using Register8<T, false>::Load8;
			uint8_t Load8() const noexcept
			{
				return RegisterIndexed8::Load8(0);
			}

			using Register8<T, false>::Store8;
			void Store8(uint8_t a_Value) noexcept
			{
				return RegisterIndexed8::Store8(0, a_Value);
			}
		};
	}

	template <typename T>
	class Register : public Internal::Register8<T>
	{
		static_assert(std::is_integral_v<T>);
		static_assert(std::is_unsigned_v<T>);
	};

	using Register8  = Register<uint8_t>;
	using Register16 = Register<uint16_t>;
	using Register32 = Register<uint32_t>;
	using Register64 = Register<uint64_t>;
}

#endif