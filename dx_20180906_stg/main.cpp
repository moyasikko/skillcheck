/*******************************************************************************
* �^�C�g��:		�Q�[�����[�v�v���O����
* �v���O������:		main.cpp
* �쐬��:		AT12B272 40 ������
* �쐬��:		2018/06/18
********************************************************************************
* �X�V����:		- 2018/06/18�@������
*			- V1.00�@Initial Version
*******************************************************************************/
/*******************************************************************************
* scanf ��warning�h�~
*******************************************************************************/
#define _CRT_SECURE_NO_WARNINGS

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include <windows.h>
#include <d3dx9.h>
#include <math.h>
#include "debugPrintf.h"
#include "common.h"
#include "mydirect3d.h"
#include "texture.h"
#include "sprite.h"
#include "scene.h"
#include "input.h"
#include "system_timer.h"
#include "Debugfont.h"
#include "debug_primitive.h"
#include "fade.h"
#include "sound.h"

/*******************************************************************************
* �}�N����`
*******************************************************************************/
#define CLASS_NAME		"GameWindow"
#define WINDOW_CAPTION	"�V���[�e�B���O�Q�[��"


/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static bool Initialize(HINSTANCE hInstance, HWND hWnd, int screen_x, int screen_y);
static void Update(void);
static void Draw(void);
static void Finalize(void);

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
LPDIRECT3DTEXTURE9 g_pTexture = NULL;
static int g_BackgroundTextureIndex = TEXTURE_INVALID_INDEX;
static double g_Basetime = 0.0f;
static int g_BaseFrame = 0;
static int g_FrameCount = 0;
static double g_FPS;
static double g_StaticFrameTime = 0;
static double time = 0.0f;
static double basetime = 0.0f;

/*******************************************************************************
�֐���:	int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
����:		HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow
�߂�l:	����I��: (int)msg.wParam
����:
*******************************************************************************/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	WNDCLASS wc = {};									//�E�C���h�E�N���X�̍쐬�E������
	wc.lpfnWndProc = WndProc;							//�E�C���h�E�v���V�[�W���[
	wc.lpszClassName = CLASS_NAME;						//�E�C���h�E�N���X��
	wc.hInstance = hInstance;							//�C���X�^���X�n���h��
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			//�}�E�X�J�[�\��
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);	//�w�i�F

	RegisterClass(&wc);									//�E�C���h�E�N���X�̓o�^

	DWORD window_style = WS_OVERLAPPEDWINDOW - WS_MAXIMIZEBOX - WS_THICKFRAME;

	RECT window_rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect(&window_rect, window_style, false);

	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	int screen_x = GetSystemMetrics(SM_CXSCREEN);
	int window_x = max((screen_x - window_width) / 2, 0);

	int screen_y = GetSystemMetrics(SM_CYSCREEN);
	int window_y = max((screen_y - window_height) / 2, 0);

	HWND hWnd = CreateWindow(							//�E�C���h�E�̍쐬���E�C���h�E�n���h�������炦��
		CLASS_NAME,										//�N���X��
		WINDOW_CAPTION,									//�L���v�V����
		window_style,									//�E�C���h�E�X�^�C���t���O�i�r�b�g�j
		window_x,										//�E�C���h�E������Wx
		window_y,										//�E�C���h�E������Wy
		window_width,									//�E�C���h�E�̕�
		window_height,									//�E�C���h�E�̍���
		NULL,											//�e�E�C���h�E�̃n���h��
		NULL,											//���j���[�n���h��
		hInstance,										//�C���X�^���X�n���h��
		NULL											//�E�C���h�E���b�Z�[�W�����̎��Ɏ󂯎��|�C���^�[
	);

	ShowWindow(hWnd, nCmdShow);							//���̃E�C���h�E���J���@�E�̕\�����@��
	UpdateWindow(hWnd);

	if (!Initialize(hInstance, hWnd, screen_x, screen_y))//�Q�[���̏�����
	{
		return -1;
	}



	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				time = SystemTimer_GetTime();
				if (time - basetime < 1.0 / 60.0)
				{
					Sleep(0);
				}
				else
				{
					basetime = time;
					//�Q�[������
					Update();

					//�}�`�`��̃o�b�`���߂̊J�n
					DebugPrimitive_BatchBegin();

					//�Q�[���`��
					Draw();

					//�V�[���̃`�F�b�N
					Scene_Check();

					//�}�`�`��̃o�b�`���߂̎��s
					DebugPrimitive_BatchRun();
				}
			}
	}

	Finalize();

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
		if (MessageBox(hWnd, "�{���ɏI�����Ă�낵���ł����H", "�m�F", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
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

bool Initialize(HINSTANCE hInstance, HWND hWnd, int screen_x, int screen_y)//�Q�[���̏�����
{
	//�L�[�{�[�h�̏�����
	if (!Keyboard_Initialize(hInstance, hWnd))
	{
		return false;
	}

	GamePad_Init();

	if (!MyDirect3D_Initialize(hWnd, screen_x, screen_y))//�Q�[���̏�����
	{
		return false;
	}

	//�f�o�b�O�t�H���g�̏�����
	DebugFont_Initialize();

	//�f�o�b�O�v���~�e�B�u�̏�����
	DebugPrimitive_Initialize();

	//�X�v���C�g�̏�����
	Sprite_Initialize();

	//�Q�[���̏�����
	Scene_Initialize(SCENE_INDEX_TITLE);

	//�V�X�e���^�C�}�[�̏�����
	SystemTimer_Initialize();

	//�V�X�e���^�C�}�[�̌v���J�n
	SystemTimer_Start();
	g_Basetime = SystemTimer_GetTime();
	g_BaseFrame = 0;
	g_FrameCount = 0;
	g_FPS = 0.0f;

	//�w�i�摜�ǂݍ���
	//g_BackgroundTextureIndex = Texture_SetLoadFile("background.png", SCREEN_WIDTH, SCREEN_HEIGHT);

	//�t�F�[�h�̏�����
	Fade_Initialize();

	//�T�E���h�̏�����
	InitSound(hWnd);


	if (Texture_Load() > 0)
	{
		return false;
	}


	return true;
}
void Update(void)//�Q�[���̍X�V
{
	//FPS�̌v��
	double time = SystemTimer_GetTime();
	if (time - g_Basetime >= 1.0)
	{
		g_FPS = (g_FrameCount - g_BaseFrame) / (time - g_Basetime);
		g_Basetime = time;
		g_BaseFrame = g_FrameCount;
	}

	//�L�[�{�[�h�̃L�[�{�[�h�̍X�V
	Keyboard_Update();

	GamePad_Update();

	//�Q�[���̍X�V
	Scene_Update();

	//�t�F�[�h�̍X�V
	Fade_Update();

	g_FrameCount++;
	
}
void Draw(void)//�Q�[���̕`��
{
	
	
	//									�F					�[��			�F					0����Ԏ�O/1����
	MyDirect3D_GetDevice()-> Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(100, 80, 200, 255), 1.0f, 0);
	MyDirect3D_GetDevice()-> BeginScene();

	//Sprite_Draw(g_BackgroundTextureIndex, 0, 0);

	//�Q�[���̕`��
	Scene_Draw();

	Fade_Draw();

	DebugFont_Draw(g_FPS);

	MyDirect3D_GetDevice()-> EndScene();
	MyDirect3D_GetDevice()-> Present(NULL, NULL, NULL, NULL);
}
void Finalize(void)
{
	UninitSound();

	Keyboard_Finalize();

	GamePad_Uninit();

	Scene_Finalize();

	Sprite_Finalize();

	Texture_AllRelease();

	MyDirect3D_Finalize();
}