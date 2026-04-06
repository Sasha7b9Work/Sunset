// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
    #pragma clang diagnostic ignored "-Wold-style-cast"
    #pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
    #pragma clang diagnostic ignored "-Wpadded"
    #pragma clang diagnostic ignored "-Wglobal-constructors"
    #pragma clang diagnostic ignored "-Wcast-qual"
    #pragma clang diagnostic ignored "-Wweak-vtables"
    #pragma clang diagnostic ignored "-Winvalid-source-encoding"
    #pragma clang diagnostic ignored "-Wfloat-equal"
    #pragma clang diagnostic ignored "-Wexit-time-destructors"
    #pragma clang diagnostic ignored "-Wformat-nonliteral"
    #pragma clang diagnostic ignored "-Wmissing-prototypes"
    #pragma clang diagnostic ignored "-Wmissing-noreturn"
    #pragma clang diagnostic ignored "-Wmissing-variable-declarations"
    #pragma clang diagnostic ignored "-Wmissing-field-initializers"
    #pragma clang diagnostic ignored "-Wnon-virtual-dtor"
    #pragma clang diagnostic ignored "-Wunused-private-field"
    #pragma clang diagnostic ignored "-Wcast-align"
    #pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
    #pragma clang diagnostic ignored "-Wnested-anon-types"
    #pragma clang diagnostic ignored "-Wshift-sign-overflow"
    #pragma clang diagnostic ignored "-Winvalid-utf8"
    #pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
    #pragma clang diagnostic ignored "-Wnonportable-include-path"
    #pragma clang diagnostic ignored "-Wextra-semi"
    #pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
    #pragma clang diagnostic ignored "-Wdouble-promotion"
    #pragma clang diagnostic ignored "-Wimplicit-fallthrough"
    #pragma clang diagnostic ignored "-Wsign-conversion"
    #pragma clang diagnostic ignored "-Wcovered-switch-default"
    #pragma clang diagnostic ignored "-Wmissing-braces"
    #pragma clang diagnostic ignored "-Wdeprecated-copy-with-user-provided-copy"
#endif


typedef unsigned int       uint;
typedef unsigned short     uint16;
typedef unsigned short     uint16_t;
typedef signed   short     int16;
typedef unsigned long long uint64;
typedef unsigned char      uint8;
typedef unsigned char      uint8_t;
typedef signed char        int8;
typedef const char * const cstr;
typedef const char *       pchar;

typedef void(*pFuncVV)();
typedef void(*pFuncVI)(int);

inline void EmptyFuncVV() {};

#define _GET_BIT(value, bit)    (((value) >> (bit)) & 0x01)
#define _SET_BIT(value, bit)    ((value) |= (1 << (bit)))
#define _CLEAR_BIT(value, bit)  ((value) &= (~(1 << (bit))))

#define _bitset(bits)                               \
  ((uint8)(                                         \
  (((uint8)((uint)bits / 0x000001) % 0x08) << 0) |  \
  (((uint8)((uint)bits / 0x000008) % 0x08) << 1) |  \
  (((uint8)((uint)bits / 0x000040) % 0x08) << 2) |  \
  (((uint8)((uint)bits / 0x000200) % 0x08) << 3) |  \
  (((uint8)((uint)bits / 0x001000) % 0x08) << 4) |  \
  (((uint8)((uint)bits / 0x008000) % 0x08) << 5) |  \
  (((uint8)((uint)bits / 0x040000) % 0x08) << 6) |  \
  (((uint8)((uint)bits / 0x200000) % 0x08) << 7)))

#define BIN_U8(bits) (_bitset(0##bits))

#define BIN_U16(bits1, bits0) ((BIN_U8(bits1) << 8) | (BIN_U8(bits0)))

#define BIN_U32(bits3, bits2, bits1, bits0) (BIN_U8(bits3) << 24 | BIN_U8(bits2) << 16 | BIN_U8(bits1) << 8 | BIN_U8(bits0))

#define HEX_FROM_2(hex1, hex0) ((uint)(0x##hex1) << 16 | (uint)0x##hex0)


#ifdef WIN32
    #define __attribute__(x)
#else
    #ifndef NULL
        #define NULL 0
    #endif
#endif


#define MAX_UINT ((uint)-1)

#define ERROR_VALUE_FLOAT   1.111e29f
#define ERROR_VALUE_UINT8   255
#define ERROR_STRING_VALUE  "--.--"

#include "Utils/Log.h"
