/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include <d3dx9.h>
#include <stdio.h>
#include "mydirect3d.h"
#include "common.h"

/*******************************************************************************
* グローバル変数
*******************************************************************************/
static LPD3DXFONT g_pD3DXFont = NULL;


void DebugFont_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	D3DXCreateFont(pDevice, 18, 0, 0, 0, false, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pD3DXFont);
}
void DebugFont_Finalize(void)
{
	if (g_pD3DXFont)
	{
		g_pD3DXFont->Release();
		g_pD3DXFont = NULL;
	}
}
void DebugFont_Draw(double fps)
{
	char font[9];
	snprintf(font, 9,"%4.2ffps", fps);
	RECT rect = { SCREEN_WIDTH - 70, SCREEN_HEIGHT-20, SCREEN_WIDTH, SCREEN_HEIGHT };
	g_pD3DXFont->DrawTextA(NULL, font, -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}
