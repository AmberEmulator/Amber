#ifndef H_DEMU_CLIENT_API
#define H_DEMU_CLIENT_API

// TODO: proper dll imports

#ifndef CLIENT_API
#if defined(CLIENT_DLL_EXPORTS)
#define CLIENT_API /*DLL_EXPORT*/
#elif defined(CLIENT_DLL)
#define CLIENT_API /*DLL_IMPORT*/
#endif
#endif

#ifndef CLIENT_API
#define CLIENT_API
#endif

#endif