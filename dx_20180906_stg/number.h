#ifndef NUMBER_H_
#define NUMBER_H_

#define NUMBER_WIDTH	(24)
#define NUMBER_HEIGHT	(24)
#define RANKING (10)

void Number_Initialize(void);
void Number_Finalize(void);
//0 <= n <= 9
void Number_Draw(float x, float y, int n, float size);

#endif