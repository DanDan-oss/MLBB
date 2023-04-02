#include "resource.h"
#include "Message.h"
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

#pragma comment(lib, "ws2_32.lib ")

#define _CRT_SECURE_NO_WARNINGS
#define GameName L"CGHK.exe"
#define DllPathW L"F:\\Document\\VCCode\\Hunter\\MLBB\\MLBBDLL\\Release\\MLBB.dll"
#define GameClassNameA "魔力寶貝"
#define GameWndTitleA "魔力寶貝"
#define SERVER_ADDR "192.168.88.128"
#define SERVER_PORT 7890


int InitServer(const char* serverIP, int severPort);		// 连接服务器
void CloseServer(int socketFd);


typedef struct
{
	HWND hwndWindow;	// 窗口句柄
	DWORD dwProcessID;	// 进程ID
}EnumWindowsArg;		// 循环遍历游戏窗口句柄时用

BOOL CALLBACK DialogProc(HWND,UINT, WPARAM, LPARAM);			// 消息回调
BOOL CALLBACK ChildFunc(HWND, UINT, WPARAM, LPARAM);			// 子控件消息点击
DWORD GetProcessID(WCHAR*);										// 任务管理器遍历获取进程PID
DWORD CALLBACK InjectDll(DWORD);								// DLL注入
DWORD CALLBACK UnloadDll(DWORD PID);							// DLL卸载
DWORD WINAPI InitDllThread(LPVOID);								// 获取DLL窗口
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);		// 枚举窗口进程的回调例程

HWND dlgHwnd = 0;												// 当前窗口句柄
HINSTANCE HInstance = 0;										// 当前应用程序句柄

DWORD PID = 0;													// 目标进程PID
HWND DllHwnd = 0;												// DLL窗口句柄
HICON hIcon = NULL;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	SOCKET sockFd = 0;

	sockFd = InitServer(SERVER_ADDR, SERVER_PORT);
	if (-1 == sockFd)
		return -1;

	hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_ICON);
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DLG_MAIN), NULL, DialogProc);

	CloseServer(sockFd);
	return 0;
}

BOOL CALLBACK DialogProc(HWND hwndDlg, UINT UMsg, WPARAM wParam, LPARAM lParam)
{
	switch (UMsg)
	{
	case WM_INITDIALOG:
		dlgHwnd = hwndDlg;
		SetWindowTextW(GetDlgItem(hwndDlg, IDC_EDIT_DLLPATH), DllPathW);
		SetWindowLong(hwndDlg, GWL_STYLE, GetWindowLong(hwndDlg, GWL_STYLE) | WS_SIZEBOX);
		SendMessage(hwndDlg, WM_SETICON, ICON_BIG, (long)hIcon);
		return 0;

	case WM_COMMAND:
		ChildFunc(hwndDlg, UMsg, wParam, lParam);
		return 0;

	case WM_CLOSE:
		DestroyWindow(hwndDlg);
		return 0;

	case WM_DESTROY:
		if (DllHwnd)
		{
			UnloadDll(PID);
		}
		
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}
	

	return 0;
}


BOOL CALLBACK ChildFunc(HWND hwndDlg, UINT UMsg, WPARAM wParam, LPARAM lParam)
{
	UINT uStance = 0;
	CHAR strPID[10] = { 0 };
	unsigned int dwPid = 0;
	switch (wParam)
	{
	case IDC_BTN_INJECT:
		if (DllHwnd)
		{
			MessageBoxW(hwndDlg, L"DllHwnd有值,DLL已载入进程", L"DLL注入", MB_ICONERROR);
			return 0;
		}
		PID = InjectDll(PID);
		sprintf(strPID, "%d", PID);
		SetWindowTextA(GetDlgItem(dlgHwnd, IDC_EDIT_PID), strPID);
		CreateThread(NULL, 0, InitDllThread, HInstance, 0, NULL);
		return 0;

	case IDC_BTN_UNLOAD:
		if (!DllHwnd)
		{
			MessageBoxW(hwndDlg, L"DLL还未加载", L"DLL卸载", MB_ICONERROR);
		}
		UnloadDll(PID);
		return 0;
	case IDC_CHECK_FIELD:
	{
		uStance = IsDlgButtonChecked(hwndDlg, IDC_CHECK_FIELD);
		if (uStance == BST_CHECKED)
			SendMessage(DllHwnd, WM_CONTROL_FUNC, VK_OPEN_COLLTASK, 1);
		if (uStance == BST_UNCHECKED)
			SendMessage(DllHwnd, WM_CONTROL_FUNC, VK_OPEN_COLLTASK, 0);
		return 0;
	}
	case IDC_CHECK_FIELDK:
	{
		uStance = IsDlgButtonChecked(hwndDlg, IDC_CHECK_FIELDK);
		if (uStance == BST_CHECKED)
			SendMessage(DllHwnd, WM_CONTROL_FUNC, VK_OPEN_COLLTASK, 2);
		if (uStance == BST_UNCHECKED)
			SendMessage(DllHwnd, WM_CONTROL_FUNC, VK_OPEN_COLLTASK, 0);
		return 0;
	}
	case IDC_CHECK_INSPECT:
	{
		uStance = IsDlgButtonChecked(hwndDlg, IDC_CHECK_INSPECT);
		if (uStance == BST_CHECKED)
			SendMessage(DllHwnd, WM_CONTROL_INSPECT, VK_AUTOMONSTER, 1);
		if (uStance == BST_UNCHECKED)
			SendMessage(DllHwnd, WM_CONTROL_INSPECT, VK_AUTOMONSTER, 0);
		return 0;
	}
	case IDC_CHECK_INSPECTK:
	{
		uStance = IsDlgButtonChecked(hwndDlg, IDC_CHECK_INSPECTK);
		if (uStance == BST_CHECKED)
			SendMessage(DllHwnd, WM_CONTROL_INSPECT, VK_AUTOMONSTER, 2);
		if (uStance == BST_UNCHECKED)
			SendMessage(DllHwnd, WM_CONTROL_INSPECT, VK_AUTOMONSTER, 0);
		return 0;
	}
	case IDC_CHECK_NEWPERSON:
	{
		uStance = IsDlgButtonChecked(hwndDlg, IDC_CHECK_NEWPERSON);
		if (uStance == BST_CHECKED)
			SendMessage(DllHwnd, WM_CONTROL_FUNC, VK_OPEN_NEWPRESON, TRUE);
		if (uStance == BST_UNCHECKED)
			SendMessage(DllHwnd, WM_CONTROL_FUNC, VK_OPEN_NEWPRESON, 0);
		return 0;
	}
	case IDC_CHECK_AUTOPLAY:
	{
		uStance = IsDlgButtonChecked(hwndDlg, IDC_CHECK_AUTOPLAY);
		if (uStance == BST_CHECKED)
			SendMessage(DllHwnd, WM_CONTROL_FUNC, VK_OPEN_AUTOPLAY, GD_OPEN_PLAYMOST);
		if (uStance == BST_UNCHECKED)
			SendMessage(DllHwnd, WM_CONTROL_FUNC, VK_OPEN_AUTOPLAY, 0);
		return 0;
	}
	case IDC_CHECK_AUTOMOST:
	{
		uStance = IsDlgButtonChecked(hwndDlg, IDC_CHECK_AUTOMOST);
		if (uStance == BST_CHECKED)
			SendMessage(DllHwnd, WM_CONTROL_FUNC, VK_OPEN_AUTOPLAY, GD_OPEN_ONE);
		if (uStance == BST_UNCHECKED)
			SendMessage(DllHwnd, WM_CONTROL_FUNC, VK_OPEN_AUTOPLAY, 0);
		return 0;
	}
	case IDC_CHECK_NUMTWO:
	{
		uStance = IsDlgButtonChecked(hwndDlg, IDC_CHECK_NUMTWO);
		if (uStance == BST_CHECKED)
			SendMessage(DllHwnd, WM_CONTROL_FUNC, VK_OPEN_AUTOPLAY, GD_OPEN_TWO);
		if (uStance == BST_UNCHECKED)
			SendMessage(DllHwnd, WM_CONTROL_FUNC, VK_OPEN_AUTOPLAY, 0);
		return 0;
	}
	case IDC_CHECK_NUMTHREE:
	{
		uStance = IsDlgButtonChecked(hwndDlg, IDC_CHECK_NUMTHREE);
		if (uStance == BST_CHECKED)
			SendMessage(DllHwnd, WM_CONTROL_FUNC, VK_OPEN_AUTOPLAY, GO_OPEN_CCC);
		if (uStance == BST_UNCHECKED)
			SendMessage(DllHwnd, WM_CONTROL_FUNC, VK_OPEN_AUTOPLAY, 0);
		return 0;
	}
	case IDC_CHECK_ADDRE:
	{
		uStance = IsDlgButtonChecked(hwndDlg, IDC_CHECK_ADDRE);
		if (uStance == BST_CHECKED)
			SendMessage(DllHwnd, WM_GROUP, VK_GROUPBOSS, 5);
		if (uStance == BST_UNCHECKED)
			SendMessage(DllHwnd, WM_GROUP, VK_GROUPBOSS, 0);
		return 0;
	}

	case IDC_CHECK_ADDRC:
	{
		uStance = IsDlgButtonChecked(hwndDlg, IDC_CHECK_ADDRC);
		if (uStance == BST_CHECKED)
			SendMessage(DllHwnd, WM_GROUP, VK_GROUPMEMBER, 5);
		if (uStance == BST_UNCHECKED)
			SendMessage(DllHwnd, WM_GROUP, VK_GROUPMEMBER, 0);
		return 0;
	}

	case IDC_BTN_PIDIN:
	{
		GetWindowTextA(GetDlgItem(dlgHwnd, IDC_EDIT_PID), strPID, 10);
		sscanf(strPID, "%d", &dwPid);
		if (DllHwnd)
		{
			MessageBoxW(hwndDlg, L"DllHwnd有值,DLL已载入进程", L"DLL注入", MB_ICONERROR);
			return 0;
		}
		PID = InjectDll(dwPid);
		if (!PID)
		{
			MessageBoxW(hwndDlg, L"DLL加载失败", L"DLL加载", MB_ICONERROR);
			return 0;
		}
		CreateThread(NULL, 0, InitDllThread, HInstance, 0, NULL);
		return 0;
	}

	case IDC_CHECK_WARE:
	{
		uStance = IsDlgButtonChecked(hwndDlg, IDC_CHECK_WARE);
		if (uStance == BST_CHECKED)
			SendMessage(DllHwnd, WM_CONTROL_FUNC, VK_OPEN_TRADE, GOLD_TRANS);
		if (uStance == BST_UNCHECKED)
			SendMessage(DllHwnd, WM_CONTROL_FUNC, VK_OPEN_TRADE, 0);
		return 0;
	}

	case IDC_CHECK_TRANSFER:
	{
		uStance = IsDlgButtonChecked(hwndDlg, IDC_CHECK_TRANSFER);
		if (uStance == BST_CHECKED)
			SendMessage(DllHwnd, WM_CONTROL_FUNC, VK_OPEN_TRADE, GOLD_TRANSFER);
		if (uStance == BST_UNCHECKED)
			SendMessage(DllHwnd, WM_CONTROL_FUNC, VK_OPEN_TRADE, 0);
		return 0;
	}
	default:
		break;
	}
	return 0;
}


DWORD GetProcessID(WCHAR* szProcessName)
{
	HANDLE hProceessnap;		// 进程快照句柄
	PROCESSENTRY32 pe32 = { 0 };
	BOOL hProcess = 0;			// 是否获取到快照
	LONG processID = 0;

	hProceessnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProceessnap == INVALID_HANDLE_VALUE)
		return 0;

	pe32.dwSize = sizeof(pe32);
	hProcess = Process32First(hProceessnap, &pe32);
	while (hProcess)
	{
		if (!wcscmp(pe32.szExeFile, szProcessName))
		{
			processID = pe32.th32ProcessID;
			break;
		}
		hProcess = Process32Next(hProceessnap, &pe32);
	}

	CloseHandle(hProceessnap);
	return processID;
}

DWORD CALLBACK InjectDll(DWORD dwPid)
{
	DWORD dwProcID = 0;		// 游戏进程ID
	DWORD dwThreID = 0;		// 游戏主线程ID
	HANDLE hnProc = 0;		// 游戏进程句柄
	LPVOID pVirAdree = 0;	// 指向申请的远程空间
	HANDLE hnThre = NULL;	// 远线程句柄
	CHAR dllPathA[_MAX_PATH] = { 0 };
	GetWindowTextA(GetDlgItem(dlgHwnd, IDC_EDIT_DLLPATH), dllPathA, _MAX_PATH);
	dwProcID = dwPid;
	if (!dwPid)
		dwProcID = GetProcessID((WCHAR*)GameName);
		
	if (!dwProcID)
	{
		MessageBoxW(dlgHwnd, L"获取进程PID失败", L"GetProcessID", MB_ICONERROR);
		return  0;
	}
	hnProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcID);
	if (!hnProc)
	{
		MessageBoxW(dlgHwnd, L"打开目标进程失败", L"OpenProcess", MB_ICONERROR);
		return  0;
	}

	pVirAdree = VirtualAllocEx(hnProc, NULL, MAX_PATH, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (!pVirAdree)
	{
		MessageBoxW(dlgHwnd, L"远线程内存申请失败", L"VirtualAllocEx", MB_ICONERROR);
		return  0;
	}
	if (!WriteProcessMemory(hnProc, pVirAdree, dllPathA, lstrlenA(dllPathA)+1, NULL))
	{
		MessageBoxW(dlgHwnd, L"内存写入失败", L"WriteProcessMemory", MB_ICONERROR);
		return  0;
	}

	hnThre = CreateRemoteThread(hnProc, NULL, NULL, \
		(LPTHREAD_START_ROUTINE)LoadLibraryA, pVirAdree, NULL, NULL);
	if (!hnThre)
	{
		MessageBoxW(dlgHwnd, L"远线程载入DLL失败", L"CreateRemoteThread", MB_ICONERROR);
		return  0;
	}
	MessageBoxW(dlgHwnd, L"DLL注入成功", L"o(*￣▽￣*)ブ", MB_OK);
	if (hnThre)
		CloseHandle(hnThre);
	if (pVirAdree)
		VirtualFreeEx(hnProc, pVirAdree, NULL, MEM_RELEASE);
	if (hnProc)
		CloseHandle(hnProc);
	return dwProcID;
}

DWORD CALLBACK UnloadDll(DWORD PID)
{
	if (!DllHwnd)
	{
		MessageBoxW(NULL, L"当前DLL没有加载游戏窗口", L"DLL卸载", MB_ICONERROR);
		return 0;
	}
	SendMessage(DllHwnd, WM_CLOSE, 0, 0);
	DllHwnd = 0;
	return 0;
}


DWORD WINAPI InitDllThread(LPVOID lpParameter)
{
	EnumWindowsArg ewa = { 0 };
	if (!PID)
	{
		MessageBoxW(NULL, L"无法获取DLL句柄,游戏PID为NULL", L"枚举DLL窗口", MB_OK);
	}
	ewa.dwProcessID = PID;
	EnumWindows(EnumWindowsProc, (LPARAM)&ewa);
	if (ewa.hwndWindow)
	{
		DllHwnd = ewa.hwndWindow;
	}
	return 1;
}

// 遍历窗口获取游戏进程窗口句柄
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	EnumWindowsArg* pArg = (EnumWindowsArg*)lParam;
	WCHAR WndTitle[MAX_PATH] = { 0 };
	DWORD dwProcessID = 0;
	// 通过窗口句柄取得进程ID
	GetWindowThreadProcessId(hwnd, &dwProcessID);
	if (dwProcessID == pArg->dwProcessID)
	{
		GetWindowTextW(pArg->hwndWindow, WndTitle, MAX_PATH);
		if (lstrlenW(WndTitle))
		{
			return TRUE;
		}
		pArg->hwndWindow = hwnd;
		return FALSE;
	}
	return TRUE;
}

int InitServer(const char* serverIP, int severPort)
{
	WSADATA wsaData = { 0 };
	SOCKET sockfd = 0;
	SOCKADDR_IN sockAddr = { 0 };

	// WINDOWS soct字库绑定
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		MessageBoxW(NULL, TEXT("Windows sockct字库初始化失败"), TEXT("BODY"), MB_OK);
		return -1;
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (!sockfd)
	{
		MessageBoxW(NULL, TEXT("网络套接字初始化失败"), TEXT("本地套接字初始化"), MB_OK);
		WSACleanup();
		return -1;
	}
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(severPort);
	sockAddr.sin_addr.s_addr = inet_addr(serverIP);
	if (-1 == connect(sockfd, (const sockaddr*)&sockAddr, sizeof(struct sockaddr)))
	{
		MessageBoxW(NULL, TEXT("服务器连接失败"), TEXT("网络错误"), MB_OK);
		closesocket(sockfd);
		WSACleanup();
		return -1;
	}
	MessageBoxW(NULL, TEXT("Hello Kitty"), TEXT("服务器连接成功"), MB_OK);
	return sockfd;
}

void CloseServer(int socketFd)
{
	closesocket(socketFd);
	WSACleanup();
	MessageBoxW(NULL, TEXT("关闭服务器连接"), TEXT("网络错误"), MB_OK);
}