#include "resource.h"
#include "Message.h"
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

#pragma comment(lib, "ws2_32.lib ")

#define _CRT_SECURE_NO_WARNINGS
#define GameName L"CGHK.exe"
#define DllPathW L"F:\\Document\\VCCode\\Hunter\\MLBB\\MLBBDLL\\Release\\MLBB.dll"
#define GameClassNameA "ħ����ؐ"
#define GameWndTitleA "ħ����ؐ"
#define SERVER_ADDR "192.168.88.128"
#define SERVER_PORT 7890


int InitServer(const char* serverIP, int severPort);		// ���ӷ�����
void CloseServer(int socketFd);


typedef struct
{
	HWND hwndWindow;	// ���ھ��
	DWORD dwProcessID;	// ����ID
}EnumWindowsArg;		// ѭ��������Ϸ���ھ��ʱ��

BOOL CALLBACK DialogProc(HWND,UINT, WPARAM, LPARAM);			// ��Ϣ�ص�
BOOL CALLBACK ChildFunc(HWND, UINT, WPARAM, LPARAM);			// �ӿؼ���Ϣ���
DWORD GetProcessID(WCHAR*);										// ���������������ȡ����PID
DWORD CALLBACK InjectDll(DWORD);								// DLLע��
DWORD CALLBACK UnloadDll(DWORD PID);							// DLLж��
DWORD WINAPI InitDllThread(LPVOID);								// ��ȡDLL����
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);		// ö�ٴ��ڽ��̵Ļص�����

HWND dlgHwnd = 0;												// ��ǰ���ھ��
HINSTANCE HInstance = 0;										// ��ǰӦ�ó�����

DWORD PID = 0;													// Ŀ�����PID
HWND DllHwnd = 0;												// DLL���ھ��
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
			MessageBoxW(hwndDlg, L"DllHwnd��ֵ,DLL���������", L"DLLע��", MB_ICONERROR);
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
			MessageBoxW(hwndDlg, L"DLL��δ����", L"DLLж��", MB_ICONERROR);
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
			MessageBoxW(hwndDlg, L"DllHwnd��ֵ,DLL���������", L"DLLע��", MB_ICONERROR);
			return 0;
		}
		PID = InjectDll(dwPid);
		if (!PID)
		{
			MessageBoxW(hwndDlg, L"DLL����ʧ��", L"DLL����", MB_ICONERROR);
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
	HANDLE hProceessnap;		// ���̿��վ��
	PROCESSENTRY32 pe32 = { 0 };
	BOOL hProcess = 0;			// �Ƿ��ȡ������
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
	DWORD dwProcID = 0;		// ��Ϸ����ID
	DWORD dwThreID = 0;		// ��Ϸ���߳�ID
	HANDLE hnProc = 0;		// ��Ϸ���̾��
	LPVOID pVirAdree = 0;	// ָ�������Զ�̿ռ�
	HANDLE hnThre = NULL;	// Զ�߳̾��
	CHAR dllPathA[_MAX_PATH] = { 0 };
	GetWindowTextA(GetDlgItem(dlgHwnd, IDC_EDIT_DLLPATH), dllPathA, _MAX_PATH);
	dwProcID = dwPid;
	if (!dwPid)
		dwProcID = GetProcessID((WCHAR*)GameName);
		
	if (!dwProcID)
	{
		MessageBoxW(dlgHwnd, L"��ȡ����PIDʧ��", L"GetProcessID", MB_ICONERROR);
		return  0;
	}
	hnProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcID);
	if (!hnProc)
	{
		MessageBoxW(dlgHwnd, L"��Ŀ�����ʧ��", L"OpenProcess", MB_ICONERROR);
		return  0;
	}

	pVirAdree = VirtualAllocEx(hnProc, NULL, MAX_PATH, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (!pVirAdree)
	{
		MessageBoxW(dlgHwnd, L"Զ�߳��ڴ�����ʧ��", L"VirtualAllocEx", MB_ICONERROR);
		return  0;
	}
	if (!WriteProcessMemory(hnProc, pVirAdree, dllPathA, lstrlenA(dllPathA)+1, NULL))
	{
		MessageBoxW(dlgHwnd, L"�ڴ�д��ʧ��", L"WriteProcessMemory", MB_ICONERROR);
		return  0;
	}

	hnThre = CreateRemoteThread(hnProc, NULL, NULL, \
		(LPTHREAD_START_ROUTINE)LoadLibraryA, pVirAdree, NULL, NULL);
	if (!hnThre)
	{
		MessageBoxW(dlgHwnd, L"Զ�߳�����DLLʧ��", L"CreateRemoteThread", MB_ICONERROR);
		return  0;
	}
	MessageBoxW(dlgHwnd, L"DLLע��ɹ�", L"o(*������*)��", MB_OK);
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
		MessageBoxW(NULL, L"��ǰDLLû�м�����Ϸ����", L"DLLж��", MB_ICONERROR);
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
		MessageBoxW(NULL, L"�޷���ȡDLL���,��ϷPIDΪNULL", L"ö��DLL����", MB_OK);
	}
	ewa.dwProcessID = PID;
	EnumWindows(EnumWindowsProc, (LPARAM)&ewa);
	if (ewa.hwndWindow)
	{
		DllHwnd = ewa.hwndWindow;
	}
	return 1;
}

// �������ڻ�ȡ��Ϸ���̴��ھ��
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	EnumWindowsArg* pArg = (EnumWindowsArg*)lParam;
	WCHAR WndTitle[MAX_PATH] = { 0 };
	DWORD dwProcessID = 0;
	// ͨ�����ھ��ȡ�ý���ID
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

	// WINDOWS soct�ֿ��
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		MessageBoxW(NULL, TEXT("Windows sockct�ֿ��ʼ��ʧ��"), TEXT("BODY"), MB_OK);
		return -1;
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (!sockfd)
	{
		MessageBoxW(NULL, TEXT("�����׽��ֳ�ʼ��ʧ��"), TEXT("�����׽��ֳ�ʼ��"), MB_OK);
		WSACleanup();
		return -1;
	}
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(severPort);
	sockAddr.sin_addr.s_addr = inet_addr(serverIP);
	if (-1 == connect(sockfd, (const sockaddr*)&sockAddr, sizeof(struct sockaddr)))
	{
		MessageBoxW(NULL, TEXT("����������ʧ��"), TEXT("�������"), MB_OK);
		closesocket(sockfd);
		WSACleanup();
		return -1;
	}
	MessageBoxW(NULL, TEXT("Hello Kitty"), TEXT("���������ӳɹ�"), MB_OK);
	return sockfd;
}

void CloseServer(int socketFd)
{
	closesocket(socketFd);
	WSACleanup();
	MessageBoxW(NULL, TEXT("�رշ���������"), TEXT("�������"), MB_OK);
}