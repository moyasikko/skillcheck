/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include <Windows.h>
#include <d3d9.h>
#include "common.h"

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
static LPDIRECT3D9 g_pD3D = NULL;
static LPDIRECT3DDEVICE9 g_pDevice = NULL;


bool MyDirect3D_Init(HWND hWnd)
{
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		MessageBox(hWnd, "�Ȃ񂩃~�X���Ă��i�΁j", "g_pD3D", MB_OK);
		return false;
	}

	D3DPRESENT_PARAMETERS d3dpp = {};		//�f�o�C�X�̃p�����[�^�[
	d3dpp.BackBufferWidth = SCREEN_WIDTH;	//�o�b�N�o�b�t�@�̕�
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;	//�o�b�N�o�b�t�@�̍���
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = TRUE;					//FALSE�Ńt���X�N���[��
	d3dpp.EnableAutoDepthStencil = TRUE;	//�[�x�o�b�t�@�E�X�e���V���o�b�t�@��L���ɂ���
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;//IMMEDIATE:�x���`�}�[�N�p
	HRESULT hr;

	//FAILED(hr)	hr�����s���Ă�����^
	//SUCCEEDED(hr)	hr���������Ă�����^
	if (FAILED(hr = g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pDevice)))
	{
		MessageBox(hWnd, "�Ȃ񂩃~�X���Ă��i�΁j", "g_pDevice", MB_OK);
		return false;
	}
	if (g_pDevice == NULL)
	{
		MessageBox(hWnd, "�Ȃ񂩃~�X���Ă��i�΁j", "g_pDevice", MB_OK);
		return false;
	}
	return true;
}

void MyDirect3D_Uninit(void)
{
	if (g_pDevice != NULL)
	{
		g_pDevice->Release();
		g_pDevice = NULL;
	}
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pDevice;
}