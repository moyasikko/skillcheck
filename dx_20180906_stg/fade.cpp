/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "fade.h"
#include "mydirect3d.h"
#include "common.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/
#define FVF_FADE_VERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

/*******************************************************************************
* グローバル変数
*******************************************************************************/
typedef struct FadeVertex_tag
{
	D3DXVECTOR4 position;
	D3DCOLOR color;
}FadeVertex;

static D3DCOLOR g_Color;
static float g_AddAlpha;
static float g_Alpha;
static bool g_bOut;
static bool g_bFade;



void Fade_Initialize(void)
{
	g_Color = D3DCOLOR_RGBA(0, 0, 0, 255);
	g_AddAlpha = 0.0f;
	g_Alpha = 0.0f;
	g_bOut = false;
	g_bFade = false;
}
void Fade_Finalize(void)
{

}
void Fade_Update(void)
{
	if (g_bFade)
	{
		g_Alpha += g_AddAlpha;
		if (g_bOut)
		{
			if (g_Alpha >= 1.0f)
			{
				g_Alpha = 1.0f;
				g_bFade = false;
			}
		}
		else
		{
			if (g_Alpha <= 0.0f)
			{
				g_Alpha = 0.0f;
				g_bFade = false;
			}
		}
	}
}
void Fade_Draw(void)
{
	if (g_Alpha <= 0.00000001f)
	{
		return;
	}
	D3DXCOLOR color = g_Color;
	color.a = g_Alpha;
	FadeVertex v[] = {
		{ D3DXVECTOR4(		  0.0f,			 0.0f, 1.0f, 1.0f), color },
		{ D3DXVECTOR4(SCREEN_WIDTH,			 0.0f, 1.0f, 1.0f), color },
		{ D3DXVECTOR4(		  0.0f, SCREEN_HEIGHT, 1.0f, 1.0f), color },
		{ D3DXVECTOR4(SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f, 1.0f), color }
	};

	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	pDevice->SetFVF(FVF_FADE_VERTEX);
	pDevice->SetTexture(0, NULL);

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(FadeVertex));
}

void Fade_Start(bool bOut, int frame, D3DCOLOR color)
{
	g_Color = color;

	g_AddAlpha = 1.0f / frame;

	if (bOut)
	{
		g_Alpha = 0.0f;
	}
	else
	{
		g_Alpha = 1.0f;
		g_AddAlpha = -g_AddAlpha;
	}

	g_bOut = bOut;
	g_bFade = true;
}
bool Fade_IsFade(void)
{
	return g_bFade;
}