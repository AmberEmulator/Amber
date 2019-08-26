#ifndef H_AMBER_GAMEBOY_API
#define H_AMBER_GAMEBOY_API

#include <common/api.hpp>

#ifndef GAMEBOY_API
#if defined(GAMEBOY_DLL_EXPORTS)
#define GAMEBOY_API COMMON_API_EXPORT
#elif defined(GAMEBOY_DLL)
#define GAMEBOY_API COMMON_API_IMPORT
#endif
#endif

#ifndef GAMEBOY_API
#define GAMEBOY_API
#endif

#endif