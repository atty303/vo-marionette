#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <Windows.h>

#include "Util.h"

void hrCheck(HRESULT hr){
	if (FAILED(hr)) {
		DebugPrintf(L"failed.(HRESULT = %08X)", hr);
		exit(-1);
	}
}

void DebugPrintf(const wchar_t *format, ...){
	static wchar_t buffer[10000];

	va_list arg;

	va_start(arg, format);
	vswprintf_s(buffer, format, arg);
	va_end(arg);

	OutputDebugString(buffer);
}
