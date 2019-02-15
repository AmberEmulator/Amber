#ifndef H_DEMU_COMMON_API
#define H_DEMU_COMMON_API

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

#endif