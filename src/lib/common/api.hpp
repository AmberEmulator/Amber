#ifndef H_AMBER_COMMON_API
#define H_AMBER_COMMON_API

#include <cstdint>
#include <string_view>
#include <type_traits>

// TODO: proper dll imports
#define COMMON_DLL_IMPORT
#define COMMON_DLL_EXPORT

#ifndef COMMON_API
#if defined(COMMON_DLL_EXPORTS)
#define COMMON_API COMMON_DLL_EXPORT
#elif defined(COMMON_DLL)
#define COMMON_API COMMON_DLL_IMPORT
#endif
#endif

#ifndef COMMON_API
#define COMMON_API
#endif

// Fixed width integer literals
constexpr int8_t operator "" _i8(unsigned long long a_Value)
{
	return static_cast<int8_t>(a_Value);
}

constexpr int16_t operator "" _i16(unsigned long long a_Value)
{
	return static_cast<int16_t>(a_Value);
}

constexpr int32_t operator "" _i32(unsigned long long a_Value)
{
	return static_cast<int32_t>(a_Value);
}

constexpr int64_t operator "" _i64(unsigned long long a_Value)
{
	return static_cast<int64_t>(a_Value);
}

constexpr uint8_t operator "" _u8(unsigned long long a_Value)
{
	return static_cast<uint8_t>(a_Value);
}

constexpr uint16_t operator "" _u16(unsigned long long a_Value)
{
	return static_cast<uint16_t>(a_Value);
}

constexpr uint32_t operator "" _u32(unsigned long long a_Value)
{
	return static_cast<uint32_t>(a_Value);
}

constexpr uint64_t operator "" _u64(unsigned long long a_Value)
{
	return static_cast<uint64_t>(a_Value);
}

#endif