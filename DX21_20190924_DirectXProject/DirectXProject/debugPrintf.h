#ifndef _DEBUG_PRINTF_H_
#define _DEBUG_PRINTF_H_

#include <Windows.h>
#include <stdio.h>

//�f�o�b�O�pprintf...Visualstudio�̏o�̓E�C���h�E�ɏo�͂����
inline void DebugPrintf(const char* pFormat, ...)
{
	//����ȕ��ɏ�����Release�r���h���̓r���h����Ȃ��͈͂�ݒ�ł���
#if defined(_DEBUG) || defined(DEBUG)
	va_list argp;
	char buf[256];	//�o�b�t�@�͑傫�߂Ɏ��܂��傤
	va_start(argp, pFormat);
	vsprintf_s(buf, 256, pFormat, argp);	//�o�b�t�@�ʂƑ����������킹�邱��
	va_end(argp);
	OutputDebugStringA(buf);
#endif // _DEBUG || DEBUG
}
#endif // !_DEBUG_PRINTF_H_
