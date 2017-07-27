#pragma once

#include<Windows.h>
#include<string>
namespace catool
{
	namespace win32
	{
#ifdef UNICODE
#define NTS(c_str) L##c_str
		using string_t = std::u16string;
		inline std::u16string HRESULT_to_string(HRESULT re)
		{
			static thread_local char16_t buffer[1024 * 2];
			FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, re, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<wchar_t*>(buffer), 0, NULL);
			return std::u16string(buffer);
		}
#else
#define NTS(c_str) c_str
		using string_t = std::string;
		inline std::string HRESULT_to_string(HRESULT re)
		{
			static thread_local char buffer[1024 * 2];
			FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, re, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<char*>(buffer), 0, NULL);
			return std::string(buffer);
		}
#endif // UNICODE


		

		
	}
}
