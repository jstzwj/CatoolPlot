#pragma once

#ifdef UNICODE
#define T(c_str) L##c_str
#else
#define T(c_str) c_str
#endif // UNICODE