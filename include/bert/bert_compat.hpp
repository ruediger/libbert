#ifndef LIBBERT_COMPAT_HPP
#define LIBBERT_COMPAT_HPP

#include <cstring>
#include <cstdio>

namespace bert{
	namespace compat{

		// compat version of C99/C++0X snprintf
#if defined(_MSC_VER)
		inline int snprintf(char* buffer, size_t count, const char *fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			// msvc's _vsnprintf returns negative if buffer wasnt large enough, but may not null terminate.
			int ret = _vsnprintf(buffer, count-1, fmt, args);
			if (ret < 0)
				buffer[count-1] = '\0';
			va_end(args);
			return ret;
		}
#else
		// import std::snprintf
		using ::std::snprintf;
#endif


	}
}

#endif
