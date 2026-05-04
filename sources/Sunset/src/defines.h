// 2023/07/04 17:46:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "option.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#pragma warning (push, 0)
#include <wx/string.h>
//#include <wx/grid.h>
//#include <wx/popupwin.h>
//#include <wx/scrolwin.h>
//#include <wx/combo.h>
//#include <wx/valnum.h>
//#include <wx/datetime.h>
//#include <wx/dirctrl.h>
//#include <wx/dir.h>
//#include <wx/rawbmp.h>
//#include <wx/artprov.h>
//#include <wx/evtloop.h>
//#include <wx/uiaction.h>
//#include <wx/wfstream.h>
//#include <wx/dnd.h>
//#include <wx/msgdlg.h>
//#include <wx/splitter.h>
//#include <wx/progdlg.h>
//#include <wx/dcbuffer.h>


#include <algorithm>
#include <array>
#include <functional>
#include <memory>
#include <random>
#include <map>
#include <queue>
#include <mutex>
#include <stack>
#include <thread>
#include <iostream>
#include <errno.h>
#include <cstring>
#include <iostream>
#include <cstdint>
#include <string>
#include <cstdarg>
#include <chrono>
#include <atomic>
#include <filesystem>
#include <limits>

#ifdef WIN32
    #define NDIS_MINIPORT_MAJOR_VERSION 0
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <iphlpapi.h>
#else
    #include <ifaddrs.h>
    #include <netdb.h>
    #include <gtk/gtk.h>
    #include <glib.h>
#endif


#pragma warning(pop)


typedef const char *      pchar;
typedef const char *const pString;
typedef uint8_t           uchar;
typedef uint8_t           uint8;
typedef uint16_t          uint16;
typedef int16_t           int16;
typedef uint32_t          uint;
typedef uint64_t          uint64;
typedef int64_t           int64;
typedef int8_t            int8;


static const int BUTTON_WIDTH = 165;
static const int BUTTON_HEIGHT = 25;
static const int TEXTCNTRL_HEIGHT = 21;
static const int WIDTH_COMBO = 180;

union BitSet128
{
    BitSet128(uint64 _value_hi = 0, uint64 _value_low = 0)
    {
        word64[0] = _value_low;
        word64[1] = _value_hi;
    }
    uint64 word64[2];
    uint   word[4];
    uint16 half_word[8];
    uint8  bytes[16];

    wxString ToASCII() const;

    void FromASCII(const wxString &);

    bool operator==(const BitSet128 &) const;
    bool operator!=(const BitSet128 &) const;
};

union BitSet16
{
    BitSet16(uint16 value)
    {
        u16 = value;
    }
    uint8  u8[2];
    uint16 u16;
};

static pString REVERSE_PROMT = "-> ";
static pString DIRECT_PROMT = "<- ";

#define SAFE_DELETE(ptr)  do { delete ptr; ptr = nullptr; } while(0)

#define L(x) wxT(x)

// Яркость элементов управления
#ifdef WIN32
    #define LIGHTNESS 300
#else
    #define LIGHTNESS 140
#endif

#include "Utils/Log.h"

extern uint VERSION_BUILD;
extern pchar DATE_BUILD;


#define IGNORE_RESULT(x) do { if (x) {} } while(0)

#include "globals.h"
