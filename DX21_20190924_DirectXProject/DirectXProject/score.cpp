/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "number.h"


void Score_Init(void)
{
	Number_Init();
}

void Score_Draw(int score, float x, float y, int digit, bool zero = true, bool left = false)
{
	int l_score = score;
	for (int i = 0; i < digit; i++)
	{
		Number_Draw(l_score % 10, x + (digit - i) * NUMBER_SIZE, y);
		l_score /= 10;
	}
}
