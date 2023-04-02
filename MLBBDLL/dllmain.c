// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include <Windows.h>
#include "Hunter.h"
#include "Message.h"

#define DllWinClassW  L"Hunter"

typedef struct
{
	HWND hwndWindow;	// 窗口句柄
	DWORD dwProcessID;	// 进程ID
}EnumWindowsArg;		// 循环遍历游戏窗口句柄时用

HMODULE hDll = 0;                        // DLL句柄
HWND hWindHnad = 0;                     // DLL窗口句柄
HWND hInWindHand = 0;                   // 游戏窗口句柄
WNDPROC OldProc = 0;		            // 游戏窗口原本的消息回调
LRESULT CALLBACK NewWndProc(HWND, UINT, WPARAM, LPARAM);// 要替换游戏De窗口过程

DWORD WINAPI InitWindow(LPVOID);                        // DLL初始化函数
LRESULT CALLBACK DllWndProc(HWND, UINT, WPARAM, LPARAM);    // DLL窗口消息回调
BOOL CALLBACK EnumWindowsProc(HWND, LPARAM);             // 获取游戏窗口句柄回调
LRESULT CALLBACK FuncKeyDown(HWND, UINT, WPARAM, LPARAM);   // 键盘消息处理函数


HANDLE DLLThread = 0;					// 当前DLL主线程句柄
HANDLE FuncThread = NULL;               //  当前运行的主任务线程
HANDLE InpectThread = NULL;				//  游戏检测线程
UCHAR IsWindBan = 0;                    //  是否锁定窗口(0不锁,1锁)

LRESULT CALLBACK FuncControl(HWND, UINT, WPARAM, LPARAM);	//前台程序任务流程

extern DWORD dwAutoFastMost;			// Hunter.c  打怪例程是否跳过打怪动画
extern DWORD dwFastField;				// Hunter.c  采集加速
extern DWORD dwRanks;					// 当前是否组队(1是队长, 2是队员)
extern DWORD dwRankNum;					// 当前是否组队(1是队长, 2是队员)

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		hDll = hModule;
		DLLThread = CreateThread(NULL, 0, InitWindow, hModule, 0, NULL);
		WinPrintA("[dbg//%s]: DLL正在注入, 线程句柄%d\n", playerName, DLLThread);
		break;
	}
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		WinPrintA("[dbg//%s]: DLL正在卸载\n", playerName);
		break;
	}
	return TRUE;
}

DWORD WINAPI InitWindow(LPVOID lpParameter)
{
	WNDCLASSW WndClass;					// 窗口类
	HWND hWnd;							// 窗口句柄
	MSG msg;							// 消息
	HINSTANCE hInstance = 0;			// 当前进程实例句柄
	unsigned int pid = 0;				// 当前进程PID
	EnumWindowsArg ewa = { 0 };
	hInstance = GetModuleHandleW(0);
	pid = GetCurrentProcessId();
	WndClass.style = NULL;
	WndClass.lpfnWndProc = DllWndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIconW(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursorW(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = DllWinClassW;
	ewa.dwProcessID = pid;

	EnumWindows(EnumWindowsProc, (LPARAM)&ewa);
	if (!hInWindHand)
	{
		WinPrintA("[dbg//%s]: 游戏窗口句柄获取失败%d\n", playerName, hInWindHand);
		return 0;
	}

	if (!RegisterClassW(&WndClass))
	{
		MessageBoxW(NULL, TEXT("DLL环境配置错误"), DllWinClassW, MB_ICONERROR);
		return 0;
	}
	hWnd = CreateWindowW(WndClass.lpszClassName, NULL, NULL, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);
	if (!hWnd)
	{
		MessageBoxW(NULL, TEXT("DLL环境初始化失败"), DllWinClassW, MB_ICONERROR);
		return 0;
	}
	UpdateWindow(hWnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	DLLThread = 0;
	WinPrintA("[dbg//%s]: 主线程正在退出\n", "DLL退出");
	UnregisterClass(WndClass.lpszClassName, WndClass.hInstance);
	FreeLibraryAndExitThread(hDll, 0);
	return msg.wParam;
}

LRESULT CALLBACK DllWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		WinPrintA("[dbg//%s]: DLL回调例程创建 游戏窗口句柄 %x DLL窗口 %x\n", playerName, hInWindHand, hwnd);
		if (hInWindHand)
		{
			OldProc = (WNDPROC)GetWindowLong(hInWindHand, GWL_WNDPROC);//获取旧的窗口过程函数指针保存
			SetWindowLongW(hInWindHand, GWL_WNDPROC, (long)NewWndProc);
		}
		break;
	}
	case WM_CONTROL_FUNC:
	{
		FuncControl(hwnd, message, wParam, lParam);
		break;
	}
	case WM_CONTROL_INSPECT:
	{
		if (wParam == VK_AUTOMONSTER)
		{
			if (lParam == 1)
			{
				WinPrintA("[dbg//%s]: [VM_AUTOMONSTER] 监测数据开启自动打怪(不跳动画)\n", playerName);
				InpectThread = OpenFunc(OPEN_FUNC_INSPECT);
				dwAutoFastMost = 0;
				return 0;
			}
			if (lParam == 2)
			{
				WinPrintA("[dbg//%s]: [VM_AUTOMONSTER] 监测数据开启自动打怪(跳动画)\n", playerName);
				InpectThread = OpenFunc(OPEN_FUNC_INSPECT);
				dwAutoFastMost = 1;
				return 0;
			}
			if (!lParam)
			{
				if (InpectThread)
					CloseFunc(InpectThread);
				InpectThread = 0;
				WinPrintA("[dbg//%s]: [VM_AUTOMONSTER] 监测数据关闭\n", playerName);
				return 0;
			}
		}
	}
	case WM_GROUP:
	{
		dwRanks = wParam;
		dwRankNum = lParam;
		break;
	}
	case WM_SETWINDOW_BOTTON:
	{
		if (lParam)
			IsWindBan = 1;
		else
			IsWindBan = 0;
		return 0;
	}
	case WM_CLOSE:
	{
		WinPrintA("[dbg//%s]: 收到要求卸载DLL消息\n", playerName);
		SetWindowLong(hInWindHand, GWL_WNDPROC, (long)OldProc);
		if(InpectThread)
			CloseFunc(InpectThread);
		if(FuncThread)
			CloseFunc(FuncThread);
		DestroyWindow(hwnd);
		return 0;
	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
		break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	EnumWindowsArg* pArg = (EnumWindowsArg*)lParam;

	DWORD dwProcessID = 0;
	// 通过窗口句柄取得进程ID
	GetWindowThreadProcessId(hwnd, &dwProcessID);
	if (dwProcessID == pArg->dwProcessID)
	{
		WinPrintA("[dbg//]: 游戏窗口句柄 %d\n", hwnd);
		hInWindHand = hwnd;
		return FALSE;
	}
	return TRUE;
}

LRESULT CALLBACK NewWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{ 
	case WM_LBUTTONDOWN:
	{

		if (IsWindBan)
			return 0;
		break;
	}
	case WM_LBUTTONDBLCLK:		// 游戏窗口被双击
	{
		if (IsWindBan)
			return 0;
		break;
	}
	case WM_SETREDRAW:
		WinPrintA("[dbg//]: 游戏窗口重绘消息 %d\n", hwnd);
		break;
	default:
		break;
	}
	return CallWindowProc(OldProc, hwnd, message, wParam, lParam);
}


LRESULT CALLBACK FuncControl(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case VK_OPEN_COLLTASK:
	{
		if (!lParam)
		{
			WinPrintA("[dbg//%s]: [VK_OPEN_COLLTASK] 采集功能关闭\n", playerName);
			if (FuncThread)
				CloseFunc(FuncThread);
			FuncThread = 0;
			return 0;
		}
		if (lParam ==1)
		{
			WinPrintA("[dbg//%s]: [VK_OPEN_COLLTASK] 采集功能开启(不加速)\n", playerName);
			dwFastField = 0;
			FuncThread = OpenFunc(OPEN_FUNC_COLLTASK);
			return 0;
		}
		if (lParam == 2)
		{
			WinPrintA("[dbg//%s]: [VK_OPEN_COLLTASK] 采集功能开启(加速)\n", playerName);
			FuncThread = OpenFunc(OPEN_FUNC_COLLTASK);
			dwFastField = 1;
			return 0;
		}
	
	}
	case VK_OPEN_NEWPRESON:
	{
		if (lParam)
		{
			WinPrintA("[dbg//%s]: [VK_OPEN_NEWPRESON] 新手任务过图开启\n", playerName);
			FuncThread = OpenFunc(OPEB_FUNC_NEWPRESON);
			return 0;
		}
		WinPrintA("[dbg//%s]: [VK_OPEN_NEWPRESON] 新手任务过图关闭\n", playerName);
		if (FuncThread)
			CloseFunc(FuncThread);
		FuncThread = 0;
		return 0;
	}
	case VK_OPEN_AUTOPLAY:
	{
		if (!(lParam))
		{
			WinPrintA("[dbg//%s]: [VK_OPEN_NEWPRESON] 挂机打怪关闭\n", playerName);
			if (FuncThread)
				CloseFunc(FuncThread);
			FuncThread = 0;
			return 0;
		}
		if (lParam == GD_OPEN_PLAYMOST)
		{
			WinPrintA("[dbg//%s]: [VK_OPEN_NEWPRESON] 根据等级自动选择挂机地点打怪\n", playerName);
			FuncThread = OpenFunc(OPEN_FUNC_AUTOPALYMOST);
			return 0;
		}
		if (lParam == GD_OPEN_ONE)
		{
			WinPrintA("[dbg//%s]: [VK_OPEN_NEWPRESON] 1-5级挂机点打怪开启\n", playerName);
			FuncThread = OpenFunc(OPEN_FUNC_AUTOPALYMOSTA);
			return 0;
		}
		if (lParam == GD_OPEN_TWO)
		{
			WinPrintA("[dbg//%s]: [VK_OPEN_NEWPRESON] 5-10级挂机点打怪开启\n", playerName);
			FuncThread = OpenFunc(OPEN_FUNC_AUTOPALYMOSTB);
			return 0;
		}
		if (lParam == GO_OPEN_CCC)
		{
			WinPrintA("[dbg//%s]: [VK_OPEN_NEWPRESON] 猎人村挂机点打怪开启\n", playerName);
			FuncThread = OpenFunc(OPEN_FUNC_AUTOPALYMOSTC);
			return 0;
		}
		return 0;
	}
	case VK_OPEN_TRADE:
	{
		if (lParam == GOLD_TRANS)
		{
			WinPrintA("[dbg//%s]: [VK_OPEN_TRADE, GOLD_TRANS] 仓库号自动接收物品开启\n", playerName);
			FuncThread = OpenFunc(OPEN_FUNC_TRADD);
			return 0;
		}
		if (lParam == GOLD_TRANSFER)
		{
			WinPrintA("[dbg//%s]: [VK_OPEN_TRADE, GOLD_TRANSFER] 仓库号开始交易\n", playerName);
			FuncThread = OpenFunc(OPEN_FUNC_TRANSFER);
			return 0;
		}
		WinPrintA("[dbg//%s]: [VK_OPEN_TRADE] 交易函数关闭\n", playerName);
		if (FuncThread)
			CloseFunc(FuncThread);
		FuncThread = 0;
		return 0;
	}
	default:
		break;
	}
	return 0;
}