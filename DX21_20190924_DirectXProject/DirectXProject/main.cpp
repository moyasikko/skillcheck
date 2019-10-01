/*******************************************************************************
* �^�C�g��:		�X�V���Ԃ̌v��
* �v���O������:		main.cpp
* �쐬��:		AT12E353 40 ������
* �쐬��:		2019/05/07
********************************************************************************
* �X�V����:		- 2019/09/10�@������
*			- V1.00�@Initial Version
*
*			- V1.01
*				UpdateWindow
*				�Q�[�����[�v
*			- V1.02
*				Init etc...
*******************************************************************************/

/*
#pragma comment(lib, "d3d9.lib")
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "d3dx9d.lib")
#else
#pragma comment(lib, "d3dx9.lib")
#endif
*/

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include <windows.h>
#include "debugPrintf.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "common.h"
#include "mydirect3d.h"
#include "texture.h"
#include "sprite.h"
#include "system_timer.h"
#include "debug_font.h"
#include "patternanimation.h"
#include "input.h"
#include "sound.h"
#include "scene.h"
#include "debug.h"
#include "fade.h"

/*******************************************************************************
* �}�N����`
*******************************************************************************/
#define CLASS_NAME		"GameWindow"
#define WINDOW_CAPTION	"����[�Ă��񂮁����[��"
#define WINDOW_STYLE	(WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX))	//�E�C���h�E�̃X�^�C��
#define FPS_MEASUREMENT_TIME (1.0)

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool Init(HINSTANCE hInstance, HWND hWnd);
void Uninit(void);
void Update(void);
void Draw(void);

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
static LPDIRECT3DDEVICE9 g_pDevice = NULL;
static int g_Texture[10];

static int g_FrameCount = 0;			//�t���[���J�E���^�[
static int g_FPSBaseFrameCount = 0;		//FPS�v���̊�ƂȂ�t���[���J�E���^�[
static double g_FPSBaseTime = 0.0;		//FPS�v���̊�ƂȂ鎞��
static double g_StaticFrameTime = 0.0;	//�t���[���Œ�p�v������
static float g_FPS = 0.0f;				//FPS

int  APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	WNDCLASS wc = {};	//�O������
	wc.lpfnWndProc = WndProc;	//�E�C���h�E�v���V�[�W���[�̓o�^�@WndProc�F�֐��|�C���^�[
	wc.lpszClassName = CLASS_NAME;	//�N���X��
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);

	RegisterClass(&wc);

	RECT window_rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

	AdjustWindowRect(&window_rect, WINDOW_STYLE, FALSE);

	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	//�v���C�}���[���j�^�[�̉�ʉ𑜓x�̎擾
	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);

	int window_x = max((desktop_width - window_width) / 2, 0);
	int window_y = max((desktop_height - window_height) / 2, 0);

	HWND hWnd = CreateWindow(
		CLASS_NAME,				//�N���X��
		WINDOW_CAPTION,			//�E�B���h�E�̃^�C�g��
		WINDOW_STYLE,			//�E�C���h�E�̃X�^�C��
		window_x,			//�����X���W
		window_y,			//�����Y���W
		window_width,			//��
		window_height,			//����
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//�Q�[�����[�v
	MSG msg = {};
	if(!Init(hInstance, hWnd))//������
	{
		return -1;
	}
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else //�Q�[������
		{
			double time = SystemTimer_GetTime();
			if (time - g_StaticFrameTime < 1.0 / 60.0)
			{
				Sleep(0);
			}
			else
			{		
				g_StaticFrameTime = time;

				Update();//�X�V

				Draw();//�`��

				if (Scene_Change())
				{
					break;
				}
			}
		}

	}
	Uninit();//�I��
	
	/* �I�� */
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		}
		break;

	case WM_CLOSE:
		if (MessageBox(hWnd, "�I�����Ă�낵�����H", "�m�F", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
		{
			DestroyWindow(hWnd);
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	};

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool Init(HINSTANCE hInstance, HWND hWnd)
{
	MyDirect3D_Init(hWnd);

	g_pDevice = GetDevice();

	int error = Texture_Load();
	if (error > 0)
	{
		MessageBox(hWnd, "�Ȃ񂩃~�X���Ă��i�΁j", "g_pTexture", MB_OK);
		return false;
	}
	else if (error < 0)
	{
		MessageBox(hWnd, "�~���肷���i�΁j", "g_pTexture", MB_OK);
		return false;
	}
	PatternAnimation_Init(hWnd);

	g_pDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 16);
	g_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	g_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	g_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	g_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	g_pDevice->SetSamplerState(0, D3DSAMP_BORDERCOLOR, D3DCOLOR_RGBA(128, 128, 128, 255));

	g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);//�A���t�@�u�����h��L���ɂ���
	//������`�悷��RGB�~������`�悷�郿�{��ʂ�RGB�~�i1-������`�悷�郿�j�@���l��0�`1
	g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);//MODURATE:����,SELECTARG1:�e�N�X�`���[,SELECTARG2:�|���S��
	g_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);//D3DTA_CURRENT�|���S���J���[

	DebugFont_Initialize();

	Keyboard_Initialize(hInstance, hWnd);

	GamePad_Init();

	InitSound(hWnd);

	SpriteInit();

	Debug_Init();

	Fade_Init();

	Scene_Init();

	//�V�X�e���^�C�}�[�̏�����
	SystemTimer_Initialize();
	//�V�X�e���^�C�}�[�̋N��
	SystemTimer_Start();
	//FPS�v���p�ϐ��Q�̏�����
	g_FrameCount = g_FPSBaseFrameCount = 0;
	g_FPS = 0.0f;
	g_StaticFrameTime = g_FPSBaseTime = SystemTimer_GetTime();

	return true;
}

void Uninit(void)
{
	Fade_Uninit();

	Debug_Uninit();

	SpriteUninit();

	DebugFont_Finalize();

	Keyboard_Finalize();

	GamePad_Uninit();

	UninitSound();

	Texture_Destroy();

	MyDirect3D_Uninit();
}

void Update()
{
	Fade_Update();

	Keyboard_Update();

	GamePad_Update();

	Scene_Update();
	
	//�t���[���J�E���g�̍X�V
	g_FrameCount++;
	//���݂̃V�X�e�����Ԃ̎擾
	double time = SystemTimer_GetTime();
	//�O���FPS�v�����Ԃ��K�莞�Ԍo���Ă�����FPS�v��
	if (time - g_FPSBaseTime >= FPS_MEASUREMENT_TIME)
	{
		//FPS�̌v�Z�i�O�񂩂�̌o�߃t���[�� / �o�ߎ��ԁj
		g_FPS = (float)((g_FrameCount - g_FPSBaseFrameCount) / (time - g_FPSBaseTime));
		//FPS�v���̊�ɂȂ�ϐ����X�V
		g_FPSBaseFrameCount = g_FrameCount;
		g_FPSBaseTime = time;
	}
}

void Draw(void)
{
	//��ʂ̃N���A
	//g_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);
	g_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(100, 120, 200, 255), 1.0f, 0);
	g_pDevice->BeginScene();
	//�`���@��Draw��BeginScene�`EndScene
	Debug_Bigin();

	Scene_Draw();

	Fade_Draw();

	DebugFont_Draw(1100, 850, "%04.1fFPS", g_FPS);

	Debug_End();
	g_pDevice->EndScene();
	g_pDevice->Present(NULL, NULL, NULL, NULL);
}