#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>
#include <string>
#include <string_view>
#include <sstream>

namespace my::win32
{
    using string = std::basic_string<TCHAR>;
    using string_view = std::basic_string_view<string::value_type, string::traits_type>;
    using stringstream = std::basic_stringstream<string::value_type, string::traits_type, string::allocator_type>;
}
