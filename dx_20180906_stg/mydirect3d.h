#ifndef MYDIRECT3D_H_
#include <Windows.h>
#include <d3dx9.h>
#include <dinput.h>

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
//mydirect3dの初期化
bool MyDirect3D_Initialize(HWND hWnd, int screen_x, int screen_y);
//Direvt3Dインターフェイスおよびデバイスの取得(Create)

//mydirect3dの終了処理
void MyDirect3D_Finalize(void);
//上の2つの解放

//デバイスの取得
LPDIRECT3DDEVICE9 MyDirect3D_GetDevice(void);
#endif
