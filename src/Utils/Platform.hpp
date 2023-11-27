#ifdef _WIN32

#define PLAT(win, linux) win
#define thiscall __thiscall
#define cdecl __cdecl

#else

#define PLAT(win, linux) linux
#define thiscall __attribute__((__cdecl__))
#define cdecl __attribute__((__cdecl__))

#endif