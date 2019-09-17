#ifndef H_AMBER_COMMON_REGISTER
#define H_AMBER_COMMON_REGISTER

#include <gameboy/api.hpp>

#include <memory>
#include <type_traits>

namespace Amber::Common
{
	namespace Internal
	{
		template <typename T>
		class RegisterBase
		{
			public:
			template <typename U>
			U Load(uint8_t a_Index) const noexcept
			{
				// TODO: big endian support
				constexpr size_t start = sizeof(T) - sizeof(U);

				U value;
				std::memcpy(&value, (m_Data + start) - sizeof(U) * a_Index, sizeof(U));
				return value;
			}

			template <typename U>
			void Store(uint8_t a_Index, U a_Value) noexcept
			{
				// TODO: big endian support
				constexpr size_t start = sizeof(T) - sizeof(U);

				std::memcpy((m_Data + start) - sizeof(U) * a_Index, &a_Value, sizeof(U));
			}

			private:
			uint8_t m_Data[sizeof(T)] = {};
		};


		template <typename T>
		class RegisterIndexed16 : public RegisterBase<T>
		{
			public:
			uint16_t Load16(uint8_t a_Index) const noexcept
			{
				return Load<uint16_t>(a_Index);
			}

			void Store16(uint8_t a_Index, uint16_t a_Value) noexcept
			{
				Store<uint16_t>(a_Index, a_Value);
			}
		};

		template <typename T>
		class RegisterNotIndexed16 : public RegisterIndexed16<T>
		{
			public:
			using RegisterIndexed16::Load16;
			uint16_t Load16() const noexcept
			{
				return RegisterIndexed16::Load16(0);
			}

			using RegisterIndexed16::Store16;
			void Store16(uint16_t a_Value) noexcept
			{
				return RegisterIndexed16::Store16(0, a_Value);
			}
		};

		template <typename T>
		class RegisterIndexed8 : public std::conditional_t<sizeof(T) == 1, RegisterBase<T>, std::conditional_t<sizeof(T) == 2, RegisterNotIndexed16<T>, RegisterIndexed16<T>>>
		{
			public:
			uint8_t Load8(uint8_t a_Index) const noexcept
			{
				return Load<uint8_t>(a_Index);
			}

			void Store8(uint8_t a_Index, uint8_t a_Value) noexcept
			{
				Store<uint8_t>(a_Index, a_Value);
			}
		};

		template <typename T>
		class RegisterNotIndexed8 : public RegisterIndexed8<T>
		{
			public:
			using RegisterIndexed8::Load8;
			uint8_t Load8() const noexcept
			{
				return RegisterIndexed8::Load8(0);
			}

			using RegisterIndexed8::Store8;
			void Store8(uint8_t a_Value) noexcept
			{
				return RegisterIndexed8::Store8(0, a_Value);
			}
		};
	}

	template <typename T>
	class Register : public std::conditional_t<sizeof(T) == 1, Internal::RegisterNotIndexed8<T>, Internal::RegisterIndexed8<T>>
	{
	};

	using Register8  = Register<uint8_t>;
	using Register16 = Register<uint16_t>;
	using Register32 = Register<uint32_t>;
	using Register64 = Register<uint64_t>;
}

#endif