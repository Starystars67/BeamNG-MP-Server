// Author: lionkor

#pragma once

// This header defines unix equivalents of common win32 functions.

#ifndef __WIN32

#include <cassert>
#include <cstring>
#include <unistd.h>

// ZeroMemory is just a {0} or a memset(addr, 0, len), and it's a macro on MSVC
inline void ZeroMemory(void* dst, size_t len) {
    assert(std::memset(dst, 0, len) != nullptr);
}
// provides unix equivalent of closesocket call in win32
inline void closesocket(int socket) {
    close(socket);
}

#ifndef __try
#define __try
#endif

#ifndef __except
#define __except(x) /**/
#endif

#endif // __WIN32