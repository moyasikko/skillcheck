#ifndef TEXTURE_H_
#define TEXTURE_H_
#include <d3dx9.h>

#define TEXTURE_INVALID_INDEX (-1)

/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
//読み込むテクスチャーファイルの事前予約をする
//
//引数：pFileName ファイル名
//    ：widht     画像の幅
//    ：height　　画像の高さ
//
//戻り値：予約（テクスチャー管理）番号
//
int Texture_SetLoadFile(const char* pFileName, int width, int height);

//テクスチャーの読み込み
//
//戻り値：読み込めなかった数
//
int Texture_Load(void);

//テクスチャーの取得
LPDIRECT3DTEXTURE9 Texture_GetTexture(int index);

//テクスチャーの幅の取得
int Texture_GetWidth(int index);

//テクスチャーの高さの取得
int Texture_GetHeight(int index);

//テクスチャーの解放
//
//引数：indexes 解放したいテクスチャー管理番号が入った配列の先頭アドレス
//    ：count   解放したいテクスチャー管理番号の個数
int Texture_Release(int indexes[], int count);

//テクスチャの全解放
void Texture_AllRelease(void);

#endif