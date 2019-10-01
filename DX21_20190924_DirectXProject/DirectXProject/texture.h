#ifndef _TEXTURE_H_
#define _TEXTURE_H_
#include <d3d9.h>

int Texture_SetLoadFile(const char* pFilename, int width, int height);
/*	読み込むテクスチャーの予約
	返り値：予約番号
	pFilename：ファイル名
	width：幅
	height：高さ
*/

int Texture_Load(void);
/*	ファイルの読み込み
	返り値：読み込めなかった数
*/

LPDIRECT3DTEXTURE9 Texture_GetTexture(int id);
/*	テクスチャー番号の取得
	返り値：テクスチャーのあれ
	id：予約番号
*/

void Texture_Destroy(int ids[], int count);
/*	テクスチャーの部分開放
	ids[]：解放したいテクスチャーのidが入った配列の先頭アドレス
	count：配列の数
*/

void Texture_Destroy(void);
/*	テクスチャーの全開放
*/

int Texture_GetWidth(int id);//テクスチャーの幅取得
int Texture_GetHeight(int id);//テクスチャーの高さ取得

#endif // !_TEXTURE_H_
