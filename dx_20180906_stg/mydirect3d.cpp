/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "mydirect3d.h"
#include "common.h"

/*******************************************************************************
* グローバル変数
*******************************************************************************/
static LPDIRECT3D9 g_pD3D = NULL;
static LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;
static LPDIRECTINPUT8 g_pDInput = NULL;

//mydirect3dの初期化
bool MyDirect3D_Initialize(HWND hWnd, int screen_x, int screen_y)
{
	/*DWORD window_style = WS_OVERLAPPEDWINDOW - WS_MAXIMIZEBOX - WS_THICKFRAME;

	RECT window_rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect(&window_rect, window_style, false);

	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	int screen_x = GetSystemMetrics(SM_CXSCREEN);
	int window_x = max((screen_x - window_width) / 2, 0);

	int screen_y = GetSystemMetrics(SM_CYSCREEN);
	int window_y = max((screen_y - window_height) / 2, 0);*/

	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		//エラー
		return false;
	}
	D3DPRESENT_PARAMETERS d3dpp = {};

	d3dpp.BackBufferWidth = SCREEN_WIDTH;//ゲーム画面のサイズ
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = TRUE;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	HRESULT hr = g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice);

	if (FAILED(hr))
	{
		hr = g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice);
		if (FAILED(hr))
		{
			hr = g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice);
			if (FAILED(hr))
			{
				//取得に失敗
			}
		}
	}

	//テクスチャーサンプラー
	MyDirect3D_GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	MyDirect3D_GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	//MyDirect3D_GetDevice()->SetSamplerState(0, D3DSAMP_BORDERCOLOR, D3DCOLOR_RGBA(200,200,200, 255));
	MyDirect3D_GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	MyDirect3D_GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	MyDirect3D_GetDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	//アルファブレンド
	MyDirect3D_GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	MyDirect3D_GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//SRC ソース（描画しようとしているポリゴン）
	MyDirect3D_GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);//DEST 今までに描画されてきた画面

	MyDirect3D_GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	MyDirect3D_GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	MyDirect3D_GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	//MyDirect3D_GetDevice()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	//MyDirect3D_GetDevice()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

	return true;
}
//Direvt3Dインターフェイスおよびデバイスの取得(Create)

//mydirect3dの終了処理
void MyDirect3D_Finalize(void)
{
	if (g_pD3DDevice)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}
	if (g_pD3D)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}
//上の2つの解放

//デバイスの取得
LPDIRECT3DDEVICE9 MyDirect3D_GetDevice(void)
{
	return g_pD3DDevice;
}