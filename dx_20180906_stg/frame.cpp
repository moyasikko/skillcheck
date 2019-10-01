/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include "texture.h"
#include "sprite.h"

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
static int g_TextureIndex = { TEXTURE_INVALID_INDEX };

void Frame_Initialize(void)
{
	g_TextureIndex = Texture_SetLoadFile("./img/waku.png", 800, 600);
}
void Frame_Finalize(void)
{
	//�e�N�X�`���[�̉��
	Texture_Release(&g_TextureIndex, 1);
}
void Frame_Update(void)
{

}
void Frame_Draw(void)
{
	Sprite_Draw(g_TextureIndex, 0, 0);
}