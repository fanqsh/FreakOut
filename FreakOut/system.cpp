#include "system.h"


SystemClass::SystemClass(void)
{
	bexit = false;
	m_spacingTime = 0.0f;
	m_timer = NULL;
	m_game = NULL;
}

SystemClass::SystemClass(const SystemClass&)
{
}

SystemClass::~SystemClass(void)
{
}



//���ô��ڳ�ʼ������������һЩ��ĳ�ʼ������
//��������ֻ���ó�ʼ�����ں���
bool SystemClass::Initialize()
{
	int screenWidth = 0, screenHeight = 0;
	bool result = false;
	//��ʼ������
	InitializeWindows(screenWidth, screenHeight);

	//������ʱ������ʼ��
	m_timer = new TimerClass;
	if (!m_timer)
		return false;
	result = m_timer->Initialize();
	if (!result)
	{
		MessageBox(m_hwnd, L"m_timer->Initialize ��ʼ��ʧ�ܣ�", L"error!!", MB_OK);
		return false;
	}

	//��ʼ����Ϸ
	m_game = new GameClass;
	if (!m_game)
		return false;
	result = m_game->Initialize(800, 600, m_hwnd, m_hdc);
	if (!result)
	{
		MessageBox(m_hwnd, L"m_game->Initialize ��ʼ��ʧ�ܣ�", L"error!!", MB_OK);
		return false;
	}
	return true;
}

void SystemClass::Shutdown()
{
	//�������һЩ���ٹ���
	if (m_timer)
	{
		delete m_timer;
		m_timer = NULL;
	}

	if (m_game)
	{
		m_game->Shutdown();
		delete m_game;
		m_game = NULL;
	}

	//ִ�д��ڵ�����
	ShutdownWindows();
}

void SystemClass::Run()
{
	MSG msg;
	bool done, result = 1;

	//��ʼ����Ϣ�ṹ
	ZeroMemory(&msg, sizeof(MSG));

	//ѭ��������Ϣ����
	done = false;

	while (!done)
	{
		//����windows��Ϣ
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//���յ�WM_QUIT��Ϣ���˳�����.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			//result = bexit;	//������ESC��Ҳ�˳�����
			//��Ⱦ������������Է��ڴ˴�

			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}
	return;
}

bool SystemClass::Frame()
{
	bool result = false;

	if (bexit)
		return false;

	m_timer->Frame();
	if (m_game->m_state == GameClass::GameState::Game_Run)
	{
		result = m_game->Frame(m_hdc, m_timer->GetTime());
		if (!result)
			return false;
	}
	else
		return true;

	return true;
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	//��ȡӦ�ó�����
	ApplicationHandle = this;

	//�õ�Ӧ�ó���ʵ�����
	m_hinstance = GetModuleHandle(NULL);

	//Ӧ�ó�������
	m_applicationName = L"��������";

	//���ô��������
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

	//�ƶ��ص�������ָ����windows���Զ����ûص��������������Ϣ�¼�
	wc.lpfnWndProc		= WndProc;//�ƶ��ص�����
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= m_hinstance;
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm			= wc.hIcon;
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= m_applicationName;
	wc.cbSize			= sizeof(WNDCLASSEX);

	//ע�ᴰ����
	RegisterClassEx(&wc);

	//��ȡwindows����ֱ���
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	//����ģʽ �� 800*600
	screenWidth = 700;
	screenHeight = 700;

	posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_OVERLAPPEDWINDOW,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	//�������
	//ShowCursor(false);

	return;
}

void SystemClass::ShutdownWindows()
{
	//��ʾ���
	//ShowCursor(true);

	//�ͷŴ��ھ��
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	//�ͷ�Ӧ�ó���ʵ��
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;
	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		//��ⰴ����Ϣ
	case WM_KEYDOWN:
		if (wparam == VK_ESCAPE)
			bexit = true;
		if (wparam == VK_LEFT)
			m_game->m_moveDir.x -= 8;
		if (wparam == VK_RIGHT)
			m_game->m_moveDir.x += 8;
		if (wparam == VK_SPACE)
			m_game->m_state = GameClass::GameState::Game_Run;
		return 0;

	case WM_KEYUP:
			return 0;
	case WM_SIZE:
			return 0;

	case WM_LBUTTONDOWN:
		return 0;

	case WM_LBUTTONUP:
		return 0;

	case WM_RBUTTONDOWN:
		return 0;

	case WM_RBUTTONUP:
		return 0;

	case WM_MOUSEWHEEL:
		return 0; 


		//������Ϣ���͸�windows����ȱʡ����
	default:
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	PAINTSTRUCT ps;        // ָ��ͼ�λ����ľ�� WM_PAINT�¼���ʹ��
	switch (umessage)
	{
	case WM_PAINT: //����������Ҫ�ػ�ʱ���� 
		{
			//BeginPaint���� ���ֻ��ñ��ƻ��ķ�Χ ���ܻ����������
			ApplicationHandle->m_hdc = BeginPaint(hwnd,&ps);     //BeginPaint hdc�������˴��ڱ��ƻ������� �����ܲ���������Ļ
			// �ڴ������Լ��Ļ��� 
			ApplicationHandle->m_game->InitiSences(ApplicationHandle->m_hdc);

			EndPaint(hwnd,&ps);

			// ����
			return(0);
		} break;
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
	default:
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
}



