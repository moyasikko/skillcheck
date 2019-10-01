/*******************************************************************************
* タイトル:		更新時間の計測
* プログラム名:		main.cpp
* 作成者:		AT12E353 40 増島僚
* 作成日:		2019/05/07
********************************************************************************
* 更新履歴:		- 2019/09/10　増島僚
*			- V1.00　Initial Version
*
*			- V1.01
*				UpdateWindow
*				ゲームループ
*			- V1.02
*				Init etc...
*******************************************************************************/

/*
#pragma comment(lib, "d3d9.lib")
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "d3dx9d.lib")
#else
#pragma comment(lib, "d3dx9.lib")
#endif
*/

/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include <windows.h>
#include "debugPrintf.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "common.h"
#include "mydirect3d.h"
#include "texture.h"
#include "sprite.h"
#include "system_timer.h"
#include "debug_font.h"
#include "patternanimation.h"
#include "input.h"
#include "sound.h"
#include "scene.h"
#include "debug.h"
#include "fade.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/
#define CLASS_NAME		"GameWindow"
#define WINDOW_CAPTION	"しゅーてぃんぐ☆げーむ"
#define WINDOW_STYLE	(WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX))	//ウインドウのスタイル
#define FPS_MEASUREMENT_TIME (1.0)

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool Init(HINSTANCE hInstance, HWND hWnd);
void Uninit(void);
void Update(void);
void Draw(void);

/*******************************************************************************
* グローバル変数
*******************************************************************************/
static LPDIRECT3DDEVICE9 g_pDevice = NULL;
static int g_Texture[10];

static int g_FrameCount = 0;			//フレームカウンター
static int g_FPSBaseFrameCount = 0;		//FPS計測の基となるフレームカウンター
static double g_FPSBaseTime = 0.0;		//FPS計測の基となる時間
static double g_StaticFrameTime = 0.0;	//フレーム固定用計測時間
static float g_FPS = 0.0f;				//FPS

int  APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	WNDCLASS wc = {};	//０初期化
	wc.lpfnWndProc = WndProc;	//ウインドウプロシージャーの登録　WndProc：関数ポインター
	wc.lpszClassName = CLASS_NAME;	//クラス名
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);

	RegisterClass(&wc);

	RECT window_rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

	AdjustWindowRect(&window_rect, WINDOW_STYLE, FALSE);

	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	//プライマリーモニターの画面解像度の取得
	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);

	int window_x = max((desktop_width - window_width) / 2, 0);
	int window_y = max((desktop_height - window_height) / 2, 0);

	HWND hWnd = CreateWindow(
		CLASS_NAME,				//クラス名
		WINDOW_CAPTION,			//ウィンドウのタイトル
		WINDOW_STYLE,			//ウインドウのスタイル
		window_x,			//左上のX座標
		window_y,			//左上のY座標
		window_width,			//幅
		window_height,			//高さ
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//ゲームループ
	MSG msg = {};
	if(!Init(hInstance, hWnd))//初期化
	{
		return -1;
	}
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else //ゲーム処理
		{
			double time = SystemTimer_GetTime();
			if (time - g_StaticFrameTime < 1.0 / 60.0)
			{
				Sleep(0);
			}
			else
			{		
				g_StaticFrameTime = time;

				Update();//更新

				Draw();//描画

				if (Scene_Change())
				{
					break;
				}
			}
		}

	}
	Uninit();//終了
	
	/* 終了 */
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		}
		break;

	case WM_CLOSE:
		if (MessageBox(hWnd, "終了してよろしいか？", "確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
		{
			DestroyWindow(hWnd);
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	};

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool Init(HINSTANCE hInstance, HWND hWnd)
{
	MyDirect3D_Init(hWnd);

	g_pDevice = GetDevice();

	int error = Texture_Load();
	if (error > 0)
	{
		MessageBox(hWnd, "なんかミスってるよ（笑）", "g_pTexture", MB_OK);
		return false;
	}
	else if (error < 0)
	{
		MessageBox(hWnd, "欲張りすぎ（笑）", "g_pTexture", MB_OK);
		return false;
	}
	PatternAnimation_Init(hWnd);

	g_pDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 16);
	g_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	g_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	g_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	g_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	g_pDevice->SetSamplerState(0, D3DSAMP_BORDERCOLOR, D3DCOLOR_RGBA(128, 128, 128, 255));

	g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);//アルファブレンドを有効にする
	//今から描画するRGB×今から描画するα＋画面のRGB×（1-今から描画するα）　※値は0～1
	g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);//MODURATE:両方,SELECTARG1:テクスチャー,SELECTARG2:ポリゴン
	g_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);//D3DTA_CURRENTポリゴンカラー

	DebugFont_Initialize();

	Keyboard_Initialize(hInstance, hWnd);

	GamePad_Init();

	InitSound(hWnd);

	SpriteInit();

	Debug_Init();

	Fade_Init();

	Scene_Init();

	//システムタイマーの初期化
	SystemTimer_Initialize();
	//システムタイマーの起動
	SystemTimer_Start();
	//FPS計測用変数群の初期化
	g_FrameCount = g_FPSBaseFrameCount = 0;
	g_FPS = 0.0f;
	g_StaticFrameTime = g_FPSBaseTime = SystemTimer_GetTime();

	return true;
}

void Uninit(void)
{
	Fade_Uninit();

	Debug_Uninit();

	SpriteUninit();

	DebugFont_Finalize();

	Keyboard_Finalize();

	GamePad_Uninit();

	UninitSound();

	Texture_Destroy();

	MyDirect3D_Uninit();
}

void Update()
{
	Fade_Update();

	Keyboard_Update();

	GamePad_Update();

	Scene_Update();
	
	//フレームカウントの更新
	g_FrameCount++;
	//現在のシステム時間の取得
	double time = SystemTimer_GetTime();
	//前回のFPS計測時間が規定時間経っていたらFPS計測
	if (time - g_FPSBaseTime >= FPS_MEASUREMENT_TIME)
	{
		//FPSの計算（前回からの経過フレーム / 経過時間）
		g_FPS = (float)((g_FrameCount - g_FPSBaseFrameCount) / (time - g_FPSBaseTime));
		//FPS計測の基になる変数を更新
		g_FPSBaseFrameCount = g_FrameCount;
		g_FPSBaseTime = time;
	}
}

void Draw(void)
{
	//画面のクリア
	//g_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);
	g_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(100, 120, 200, 255), 1.0f, 0);
	g_pDevice->BeginScene();
	//描く　※DrawのBeginScene～EndScene
	Debug_Bigin();

	Scene_Draw();

	Fade_Draw();

	DebugFont_Draw(1100, 850, "%04.1fFPS", g_FPS);

	Debug_End();
	g_pDevice->EndScene();
	g_pDevice->Present(NULL, NULL, NULL, NULL);
}