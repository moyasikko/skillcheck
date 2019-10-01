/*******************************************************************************
* タイトル:		ゲームループプログラム
* プログラム名:		main.cpp
* 作成者:		AT12B272 40 増島僚
* 作成日:		2018/06/18
********************************************************************************
* 更新履歴:		- 2018/06/18　増島僚
*			- V1.00　Initial Version
*******************************************************************************/
/*******************************************************************************
* scanf のwarning防止
*******************************************************************************/
#define _CRT_SECURE_NO_WARNINGS

/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include <windows.h>
#include <d3dx9.h>
#include <math.h>
#include "debugPrintf.h"
#include "common.h"
#include "mydirect3d.h"
#include "texture.h"
#include "sprite.h"
#include "scene.h"
#include "input.h"
#include "system_timer.h"
#include "Debugfont.h"
#include "debug_primitive.h"
#include "fade.h"
#include "sound.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/
#define CLASS_NAME		"GameWindow"
#define WINDOW_CAPTION	"シューティングゲーム"


/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static bool Initialize(HINSTANCE hInstance, HWND hWnd, int screen_x, int screen_y);
static void Update(void);
static void Draw(void);
static void Finalize(void);

/*******************************************************************************
* グローバル変数
*******************************************************************************/
LPDIRECT3DTEXTURE9 g_pTexture = NULL;
static int g_BackgroundTextureIndex = TEXTURE_INVALID_INDEX;
static double g_Basetime = 0.0f;
static int g_BaseFrame = 0;
static int g_FrameCount = 0;
static double g_FPS;
static double g_StaticFrameTime = 0;
static double time = 0.0f;
static double basetime = 0.0f;

/*******************************************************************************
関数名:	int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
引数:		HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow
戻り値:	正常終了: (int)msg.wParam
説明:
*******************************************************************************/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	WNDCLASS wc = {};									//ウインドウクラスの作成・初期化
	wc.lpfnWndProc = WndProc;							//ウインドウプロシージャー
	wc.lpszClassName = CLASS_NAME;						//ウインドウクラス名
	wc.hInstance = hInstance;							//インスタンスハンドル
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			//マウスカーソル
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);	//背景色

	RegisterClass(&wc);									//ウインドウクラスの登録

	DWORD window_style = WS_OVERLAPPEDWINDOW - WS_MAXIMIZEBOX - WS_THICKFRAME;

	RECT window_rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect(&window_rect, window_style, false);

	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	int screen_x = GetSystemMetrics(SM_CXSCREEN);
	int window_x = max((screen_x - window_width) / 2, 0);

	int screen_y = GetSystemMetrics(SM_CYSCREEN);
	int window_y = max((screen_y - window_height) / 2, 0);

	HWND hWnd = CreateWindow(							//ウインドウの作成→ウインドウハンドルがもらえる
		CLASS_NAME,										//クラス名
		WINDOW_CAPTION,									//キャプション
		window_style,									//ウインドウスタイルフラグ（ビット）
		window_x,										//ウインドウ左上座標x
		window_y,										//ウインドウ左上座標y
		window_width,									//ウインドウの幅
		window_height,									//ウインドウの高さ
		NULL,											//親ウインドウのハンドル
		NULL,											//メニューハンドル
		hInstance,										//インスタンスハンドル
		NULL											//ウインドウメッセージ処理の時に受け取るポインター
	);

	ShowWindow(hWnd, nCmdShow);							//左のウインドウを開く　右の表示方法で
	UpdateWindow(hWnd);

	if (!Initialize(hInstance, hWnd, screen_x, screen_y))//ゲームの初期化
	{
		return -1;
	}



	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				time = SystemTimer_GetTime();
				if (time - basetime < 1.0 / 60.0)
				{
					Sleep(0);
				}
				else
				{
					basetime = time;
					//ゲーム処理
					Update();

					//図形描画のバッチ命令の開始
					DebugPrimitive_BatchBegin();

					//ゲーム描画
					Draw();

					//シーンのチェック
					Scene_Check();

					//図形描画のバッチ命令の実行
					DebugPrimitive_BatchRun();
				}
			}
	}

	Finalize();

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
		if (MessageBox(hWnd, "本当に終了してよろしいですか？", "確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
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

bool Initialize(HINSTANCE hInstance, HWND hWnd, int screen_x, int screen_y)//ゲームの初期化
{
	//キーボードの初期化
	if (!Keyboard_Initialize(hInstance, hWnd))
	{
		return false;
	}

	GamePad_Init();

	if (!MyDirect3D_Initialize(hWnd, screen_x, screen_y))//ゲームの初期化
	{
		return false;
	}

	//デバッグフォントの初期化
	DebugFont_Initialize();

	//デバッグプリミティブの初期化
	DebugPrimitive_Initialize();

	//スプライトの初期化
	Sprite_Initialize();

	//ゲームの初期化
	Scene_Initialize(SCENE_INDEX_TITLE);

	//システムタイマーの初期化
	SystemTimer_Initialize();

	//システムタイマーの計測開始
	SystemTimer_Start();
	g_Basetime = SystemTimer_GetTime();
	g_BaseFrame = 0;
	g_FrameCount = 0;
	g_FPS = 0.0f;

	//背景画像読み込み
	//g_BackgroundTextureIndex = Texture_SetLoadFile("background.png", SCREEN_WIDTH, SCREEN_HEIGHT);

	//フェードの初期化
	Fade_Initialize();

	//サウンドの初期化
	InitSound(hWnd);


	if (Texture_Load() > 0)
	{
		return false;
	}


	return true;
}
void Update(void)//ゲームの更新
{
	//FPSの計測
	double time = SystemTimer_GetTime();
	if (time - g_Basetime >= 1.0)
	{
		g_FPS = (g_FrameCount - g_BaseFrame) / (time - g_Basetime);
		g_Basetime = time;
		g_BaseFrame = g_FrameCount;
	}

	//キーボードのキーボードの更新
	Keyboard_Update();

	GamePad_Update();

	//ゲームの更新
	Scene_Update();

	//フェードの更新
	Fade_Update();

	g_FrameCount++;
	
}
void Draw(void)//ゲームの描画
{
	
	
	//									色					深さ			色					0が一番手前/1が奥
	MyDirect3D_GetDevice()-> Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(100, 80, 200, 255), 1.0f, 0);
	MyDirect3D_GetDevice()-> BeginScene();

	//Sprite_Draw(g_BackgroundTextureIndex, 0, 0);

	//ゲームの描画
	Scene_Draw();

	Fade_Draw();

	DebugFont_Draw(g_FPS);

	MyDirect3D_GetDevice()-> EndScene();
	MyDirect3D_GetDevice()-> Present(NULL, NULL, NULL, NULL);
}
void Finalize(void)
{
	UninitSound();

	Keyboard_Finalize();

	GamePad_Uninit();

	Scene_Finalize();

	Sprite_Finalize();

	Texture_AllRelease();

	MyDirect3D_Finalize();
}