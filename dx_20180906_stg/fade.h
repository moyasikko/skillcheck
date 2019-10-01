#ifndef FADE_H_
#define FADE_H_

#include <d3dx9.h>

void Fade_Initialize(void);
void Fade_Finalize(void);
void Fade_Update(void);
void Fade_Draw(void);

void Fade_Start(bool bOut, int frame, D3DCOLOR color);
bool Fade_IsFade(void);

#endif // !FADE_H_
