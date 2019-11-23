#pragma once

#include "linux/windows/wincompat.h"

#include <cstddef>
#include <ctype.h>

void strcpy_s(char * dest, size_t size, const char * source);

#define _strdup strdup
#define _strtoui64 strtoull
#define _vsntprintf vsnprintf
#define _tcsrchr strrchr
#define _tcsncpy strncpy
#define _tcslen     strlen
#define _tcscmp strcmp
#define _stricmp strcasecmp
#define _tcschr strchr
#define _tcsstr strstr
#define _tcscpy     strcpy
#define _snprintf snprintf
#define wsprintf sprintf

inline bool IsCharLower(char ch) {
	return isascii(ch) && islower(ch);
}

inline bool IsCharUpper(char ch) {
	return isascii(ch) && isupper(ch);
}

DWORD CharLowerBuff(LPTSTR lpsz, DWORD cchLength);
