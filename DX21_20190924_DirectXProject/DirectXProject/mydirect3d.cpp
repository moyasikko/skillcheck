/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include <Windows.h>
#include <d3d9.h>
#include "common.h"

/*******************************************************************************
* グローバル変数
*******************************************************************************/
static LPDIRECT3D9 g_pD3D = NULL;
static LPDIRECT3DDEVICE9 g_pDevice = NULL;


bool MyDirect3D_Init(HWND hWnd)
{
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		MessageBox(hWnd, "なんかミスってるよ（笑）", "g_pD3D", MB_OK);
		return false;
	}

	D3DPRESENT_PARAMETERS d3dpp = {};		//デバイスのパラメーター
	d3dpp.BackBufferWidth = SCREEN_WIDTH;	//バックバッファの幅
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;	//バックバッファの高さ
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = TRUE;					//FALSEでフルスクリーン
	d3dpp.EnableAutoDepthStencil = TRUE;	//深度バッファ・ステンシルバッファを有効にする
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;//IMMEDIATE:ベンチマーク用
	HRESULT hr;

	//FAILED(hr)	hrが失敗していたら真
	//SUCCEEDED(hr)	hrが成功していたら真
	if (FAILED(hr = g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pDevice)))
	{
		MessageBox(hWnd, "なんかミスってるよ（笑）", "g_pDevice", MB_OK);
		return false;
	}
	if (g_pDevice == NULL)
	{
		MessageBox(hWnd, "なんかミスってるよ（笑）", "g_pDevice", MB_OK);
		return false;
	}
	return true;
}

void MyDirect3D_Uninit(void)
{
	if (g_pDevice != NULL)
	{
		g_pDevice->Release();
		g_pDevice = NULL;
	}
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pDevice;
}