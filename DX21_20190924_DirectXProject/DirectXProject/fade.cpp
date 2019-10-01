/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include <d3d9.h>
#include <d3dx9.h>
#include "sprite.h"
#include "common.h"
#include "mydirect3d.h"

/*******************************************************************************
* 構造体定義
*******************************************************************************/
typedef struct FadeVertex_tag
{
	D3DXVECTOR4 position;
	D3DCOLOR color;
}FadeVertex;
#define FVF_FADE_VERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

/*******************************************************************************
* グローバル変数
*******************************************************************************/
static bool g_bFadeOut = false;
static bool g_bFade = false;
static int g_FadeFrame = 0;
static int g_FadeFrameCount = 0;
static int g_FadeStartFrame = 0;
static D3DCOLOR g_FadeColor = 0;


void Fade_Init(void)
{
	g_bFade = false;
	g_FadeFrameCount = 0;
}

void Fade_Uninit(void)
{

}

void Fade_Update(void)
{
	if (g_bFade)
	{
		int elapsedFrame = g_FadeFrameCount - g_FadeStartFrame;
		float e = elapsedFrame / (float)g_FadeFrame;
		float alpha = 0;
		if (g_bFadeOut)
		{
			alpha = e;
		}
		else
		{
			alpha = 1.0f - e;
		}
		D3DXCOLOR color = g_FadeColor;
		color.a = alpha;
		g_FadeColor = color;
		if (elapsedFrame >= g_FadeFrame)
		{
			g_bFade = false;
		}
	}
	g_FadeFrameCount++;
}

void Fade_Draw(void)
{
	D3DXCOLOR color = g_FadeColor;
	if (color.a <= 0.0001f)
		return;
	FadeVertex v[] = {
		{ D3DXVECTOR4(         0.0f,          0.0f, 0.0f, 1.0f), g_FadeColor },
		{ D3DXVECTOR4( SCREEN_WIDTH,          0.0f, 0.0f, 1.0f), g_FadeColor },
		{ D3DXVECTOR4(         0.0f, SCREEN_HEIGHT, 0.0f, 1.0f), g_FadeColor },
		{ D3DXVECTOR4( SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f), g_FadeColor }
	};
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	pDevice->SetFVF(FVF_FADE_VERTEX);
	pDevice->SetTexture(0, NULL);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(FadeVertex));
}

void Fade_Start(int frame, D3DCOLOR color, bool bFadeOut)
{
	g_FadeFrame = frame;
	g_FadeColor = color;
	g_bFadeOut = bFadeOut;
	g_FadeStartFrame = g_FadeFrameCount;
	g_bFade = true;
}

bool Fade_IsFade(void)
{
	return g_bFade;
}