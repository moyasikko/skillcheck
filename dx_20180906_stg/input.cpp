//=============================================================================
//
// 入力処理 [input.cpp]
//
//=============================================================================
#include <Windows.h>
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>
#include <Xinput.h>
#include <d3dx9.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_KEY_MAX			(256)	// キーの最大数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
static bool initialize(HINSTANCE hInstance);
static void finalize(void);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECTINPUT8			g_pInput = NULL;					// DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8	g_pDevKeyboard = NULL;				// 入力デバイス(キーボード)へのポインタ
BYTE					g_aKeyState[NUM_KEY_MAX];			// キーボードの入力情報ワーク
BYTE					g_aKeyStateTrigger[NUM_KEY_MAX];	// キーボードのトリガー情報ワーク
BYTE					g_aKeyStateRelease[NUM_KEY_MAX];	// キーボードのリリース情報ワーク

XINPUT_STATE g_state;
XINPUT_STATE g_stateTrigger;
XINPUT_STATE g_stateRelease;
//WORD g_wButtons;
BYTE g_bLeftTrigger;
BYTE g_bRightTrigger;
D3DXVECTOR2 g_sThumbL;
D3DXVECTOR2 g_sThumbR;
SHORT g_sThumbLX;
SHORT g_sThumbLY;
SHORT g_sThumbRX;
SHORT g_sThumbRY;

XINPUT_VIBRATION g_Vibration;

//=============================================================================
// 入力処理の初期化
//=============================================================================
bool initialize(HINSTANCE hInstance)
{
	if(g_pInput == NULL) {

		// DirectInputオブジェクトの作成
		if(FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL))) {

			return false;
		}
	}

	return true;
}

//=============================================================================
// 入力処理の終了処理
//=============================================================================
void finalize(void)
{
	if(g_pInput != NULL) {

		// DirectInputオブジェクトの開放
		g_pInput->Release();
		g_pInput = NULL;
	}
}


//=============================================================================
// キーボードの初期化
//=============================================================================
bool Keyboard_Initialize(HINSTANCE hInstance, HWND hWnd)
{
	// 入力処理の初期化
	if( !initialize(hInstance) ) {

		MessageBox(hWnd, "DirectInputオブジェクトが作れねぇ！", "警告！", MB_ICONWARNING);
		return false;
	}

	// デバイスの作成
	if(FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		MessageBox(hWnd, "キーボードがねぇ！", "警告！", MB_ICONWARNING);
		return false;
	}

	// データフォーマットを設定
	if(FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return false;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	if(FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(hWnd, "キーボードの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return false;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	g_pDevKeyboard->Acquire();

	return true;
}

//=============================================================================
// キーボードの終了処理
//=============================================================================
void Keyboard_Finalize(void)
{
	if(g_pDevKeyboard != NULL)
	{// 入力デバイス(キーボード)の開放
		// キーボードへのアクセス権を開放(入力制御終了)
		g_pDevKeyboard->Unacquire();

		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	// 入力処理の終了処理
	finalize();
}

//=============================================================================
// キーボードの更新処理
//=============================================================================
void Keyboard_Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];

	// デバイスからデータを取得
	if(SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		// aKeyState[DIK_SPACE] & 0x80

		for(int nCnKey = 0; nCnKey < NUM_KEY_MAX; nCnKey++)
		{
			// キートリガー・リリース情報を生成
			g_aKeyStateTrigger[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & aKeyState[nCnKey];
			g_aKeyStateRelease[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & g_aKeyState[nCnKey];


			// キープレス情報を保存
			g_aKeyState[nCnKey] = aKeyState[nCnKey];
		}
	}
	else
	{
		// キーボードへのアクセス権を取得
		g_pDevKeyboard->Acquire();
	}
}

//=============================================================================
// キーボードのプレス状態を取得
//=============================================================================
bool Keyboard_IsPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//=============================================================================
// キーボードのトリガー状態を取得
//=============================================================================
bool Keyboard_IsTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true: false;
}

//=============================================================================
// キーボードのリリ－ス状態を取得
//=============================================================================
bool Keyboard_IsRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true: false;
}

bool GamePad_Init(void)
{
	if (XInputGetState(0,       // DWORD         dwUserIndex
		&g_state) // XINPUT_STATE* pState
		== ERROR_SUCCESS
		)
	{
		g_Vibration.wLeftMotorSpeed = 0;
		g_Vibration.wRightMotorSpeed = 0;
		return true;
	}
	else
	{
		return false;
	}
}

void GamePad_Uninit(void)
{
	XInputEnable(false); // BOOL enabl
}

void GamePad_Update(void)
{
	XINPUT_STATE state;

	XInputGetState(0, &state);

	g_stateTrigger.Gamepad.wButtons = (g_state.Gamepad.wButtons^state.Gamepad.wButtons) & state.Gamepad.wButtons;
	g_stateRelease.Gamepad.wButtons = (g_state.Gamepad.wButtons^state.Gamepad.wButtons) & g_state.Gamepad.wButtons;

	g_state = state;

	if (g_state.Gamepad.sThumbLX > 7849 || g_state.Gamepad.sThumbLX < -7849)
		g_sThumbL.x = g_state.Gamepad.sThumbLX;
	else
		g_sThumbL.x = 0;
	if (g_state.Gamepad.sThumbLY > 7849 || g_state.Gamepad.sThumbLY < -7849)
		g_sThumbL.y = -g_state.Gamepad.sThumbLY;
	else
		g_sThumbL.y = 0;
}

D3DXVECTOR2 GamePad_GetThumbL(void)
{
	return g_sThumbL;
}

//=============================================================================
// ボタンのプレス状態を取得
//=============================================================================
bool GamePad_IsPress(WORD Buttons)
{
	return (g_state.Gamepad.wButtons & Buttons) ? true : false;
}

//=============================================================================
// ボタンのトリガー状態を取得
//=============================================================================
bool GamePad_IsTrigger(WORD Buttons)
{
	return (g_stateTrigger.Gamepad.wButtons & Buttons) ? true : false;
}

//=============================================================================
// ボタンのリリ－ス状態を取得
//=============================================================================
bool GamePad_IsRelease(WORD Buttons)
{
	return (g_stateRelease.Gamepad.wButtons & Buttons) ? true : false;
}

void GamePad_VibrationStart(void)
{
	g_Vibration.wLeftMotorSpeed = WORD(65535);
	g_Vibration.wRightMotorSpeed = WORD(65535);
	XInputSetState(0, &g_Vibration);
}

void GamePad_VibrationStop(void)
{
	g_Vibration.wLeftMotorSpeed = 0;
	g_Vibration.wRightMotorSpeed = 0;
	XInputSetState(0, &g_Vibration);
}

int GamePad_LTrigger(void)
{
	return g_state.Gamepad.bLeftTrigger;
}

int GamePad_RTrigger(void)
{
	return g_state.Gamepad.bRightTrigger;
}