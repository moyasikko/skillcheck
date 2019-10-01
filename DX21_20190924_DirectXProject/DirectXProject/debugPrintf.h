#ifndef _DEBUG_PRINTF_H_
#define _DEBUG_PRINTF_H_

#include <Windows.h>
#include <stdio.h>

//デバッグ用printf...Visualstudioの出力ウインドウに出力される
inline void DebugPrintf(const char* pFormat, ...)
{
	//こんな風に書くとReleaseビルド時はビルドされない範囲を設定できる
#if defined(_DEBUG) || defined(DEBUG)
	va_list argp;
	char buf[256];	//バッファは大きめに取りましょう
	va_start(argp, pFormat);
	vsprintf_s(buf, 256, pFormat, argp);	//バッファ量と第二引数を合わせること
	va_end(argp);
	OutputDebugStringA(buf);
#endif // _DEBUG || DEBUG
}
#endif // !_DEBUG_PRINTF_H_
