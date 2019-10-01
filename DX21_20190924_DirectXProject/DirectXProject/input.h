//=============================================================================
//
// キーボード入力処理 [input.h]
//
//=============================================================================
#ifndef INPUT_H_
#define INPUT_H_

#include <Windows.h>
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>
#include <d3dx9.h>
#include <Xinput.h>

bool Keyboard_Initialize(HINSTANCE hInstance, HWND hWnd);
void Keyboard_Finalize(void);
void Keyboard_Update(void);

void GamePad_Init(void);
void GamePad_Uninit(void);
void GamePad_Update(void);
D3DXVECTOR2 GamePad_GetThumbL(void);
D3DXVECTOR2 GamePad_GetThumbR(void);

// DIK_～
bool Keyboard_IsPress(int nKey);
bool Keyboard_IsTrigger(int nKey);
bool Keyboard_IsRelease(int nKey);

bool GamePad_IsPress  (WORD Buttons);
bool GamePad_IsTrigger(WORD Buttons);
bool GamePad_IsRelease(WORD Buttons);

void GamePad_VibrationStart(void);
void GamePad_VibrationStop(void);

int GamePad_LTrigger(void);
int GamePad_RTrigger(void);

bool GamePad_GetFlag(void);

#endif