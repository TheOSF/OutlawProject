#include	"iextreme.h"
#include	"Framework.h"
#include "IEX_Input.h"
#include	"../SceneOption/SceneOption.h"
#include	"../debug/DebugFunction.h"

#include "../../Input/GamePad/GamePadManager.h"
#include "../Sound/Sound.h"
#include "../Render/Renderer.h"

//*****************************************************************************************************************************
//
//
//
//*****************************************************************************************************************************

char*	AppTitle = "Outlaw Sports Festival";

BOOL	bFullScreen = FALSE;
DWORD	ScreenMode = SCREEN900p;

Framework*	MainFrame = NULL;

//*****************************************************************************************************************************
//		�A�v���P�[�V����������
//*****************************************************************************************************************************

BOOL	InitApp( HWND hWnd )
{
	//	IEX�V�X�e��������
	IEX_Initialize( hWnd, bFullScreen, ScreenMode );
	IEX_InitAudio();

    // ������
#ifdef OUTLAW2_CONTROLL_TYPE_IEX_INPUT
	IEX_InitInput();
#else
	GamePadManager::Initialize(hWnd);
#endif


	//	�V�X�e���̏�����
	SYSTEM_Initialize();
	//	���C���t���[�����[�N����
    MainFrame = new Framework(FPS_FLEX);
	//	�����V�[���o�^
	MainFrame->ChangeScene(new SceneOption());

    //�@���E�[�x�o�b�t�@��o�^
    {
        shader->SetValue("NormalDepthMap",
            DefRendererMgr.GetNormalDepthTexture()->GetTexture());
    }

    //�T�E���h������
    {
        Sound::Initialize();
    }

	return TRUE;
}

//*****************************************************************************************************************************
//		�E�B���h�E�v���V�[�W��
//*****************************************************************************************************************************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message){
	case WM_DESTROY: PostQuitMessage(0); return 0;
	case WM_KEYDOWN:
			switch (wParam){
			case VK_ESCAPE: PostMessage(hWnd, WM_CLOSE, 0, 0); return 0;
			case VK_F1:		iexSystem::OpenDebugWindow(); return 0;
			}
			break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

//*****************************************************************************************************************************
//		�E�B���h�E�쐬
//*****************************************************************************************************************************
HWND InitWindow( HINSTANCE hInstance, int nCmdShow )
{
	HWND			hWnd;

	//	�X�N���[���T�C�Y�擾
	RECT	WindowSize;
	iexSystem::GetScreenRect( ScreenMode, WindowSize );

	//	�E�B���h�E�N���X�ݒ�
	WNDCLASS	wc;
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WindowProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH )GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = "Outlaw Sports Festival";
	RegisterClass(&wc);

	//	�E�B���h�E�쐬
	if( !bFullScreen ){
		AdjustWindowRect( &WindowSize, WS_OVERLAPPEDWINDOW, FALSE );
		hWnd = CreateWindow( "Outlaw Sports Festival", AppTitle, WS_OVERLAPPEDWINDOW, 
								0, 0, WindowSize.right-WindowSize.left, WindowSize.bottom-WindowSize.top,
								NULL, NULL, hInstance, NULL);
	} else {
		hWnd = CreateWindow( "Outlaw Sports Festival", AppTitle, WS_POPUP, 0, 0, WindowSize.right,WindowSize.bottom, NULL, NULL, hInstance, NULL);
		ShowCursor(FALSE);
	}
	if( !hWnd ) return NULL;
    
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

    return hWnd;
}

//*****************************************************************************************************************************
//		WinMain
//*****************************************************************************************************************************
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	MSG		msg;
	HWND	hWnd;

	MyMemoryLeakCheck;

	if( GetAsyncKeyState(VK_CONTROL)&0x8000 ) bFullScreen = TRUE;

	hWnd = InitWindow(hInstance, nCmdShow);
	InitApp(hWnd);

	//	���C�����[�v
	for(;;){
		if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ){
			if( !GetMessage(&msg, NULL, 0, 0) ) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			if( MainFrame->Update() ) MainFrame->Render();
		}
	}

	//	�S���	
	delete	MainFrame;
	iexParticle::Release();

    DefRendererMgr.Release();

	SYSTEM_Release();
	iexSystem::CloseDebugWindow();

    // ���
#ifdef OUTLAW2_CONTROLL_TYPE_IEX_INPUT
    IEX_ReleaseInput();
#else
    GamePadManager::Release();
#endif

	IEX_ReleaseAudio();

	return 0;
}
