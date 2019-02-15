#ifndef H_DEMU_GAMEBOY_API
#define H_DEMU_GAMEBOY_API

// TODO: proper dll imports

#ifndef GAMEBOY_API
#if defined(GAMEBOY_DLL_EXPORTS)
#define GAMEBOY_API /*DLL_EXPORT*/
#elif defined(GAMEBOY_DLL)
#define GAMEBOY_API /*DLL_IMPORT*/
#endif
#endif

#ifndef GAMEBOY_API
#define GAMEBOY_API
#endif

#endif