#include "Hunter.h"
#include <strsafe.h>

#define WHITELIST_MAX 2


int aircraft[6][4] =							// ���͵�����
{ {141, 148, 141, 147}, {63, 79, 63, 78}, {242, 100, 243, 100},
 {162, 130, 163, 130}, {72, 123, 73, 123}, {233, 78, 233, 77} };

// ��������Ʒʱ������
char* PeddlIndexA[] = { (char*)"8\\z1",  (char*)"9\\z1", (char*)"10\\z1",(char*)"11\\z1", (char*)"12\\z1",
(char*)"13\\z1", (char*)"14\\z1", (char*)"15\\z1", (char*)"16\\z1", (char*)"17\\z1",
(char*)"18\\z1", (char*)"19\\z1", (char*)"20\\z1", (char*)"21\\z1", (char*)"22\\z1",
(char*)"23\\z1", (char*)"24\\z1", (char*)"25\\z1", (char*)"26\\z1", (char*)"27\\z1" };

char* StrNumA[] = { (char*)"0", (char*)"1", (char*)"2", (char*)"3" };

char RoleDirec[] = { 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48};  // ���﷽��(0-7) 'A-H'

// ������ƷID�б������(����,id)
int WhiteListID[WHITELIST_MAX][2] = 
{
	{0x6988, 0x2ee6}/*����*/ , {0x69f7, 0x3079}/*���� */
};



// NPC������Ϣ
NPCRoleClass NPCRole[] = {
	{156,122},		// �̵�NPC
	{8, 30},			// ����ҽԺ��ʿ
	{11, 18}			// ����ҽԺҽ��
};

extern HANDLE FuncThread;			// ������Ϸ�����߳�
extern HWND hWindHnad;			// DLL���ھ��

DWORD dwAutoFastMost = 0;			// ��������Ƿ�������ֹ���
DWORD dwFastField = 0;				// �Ƿ�ɼ�����
DWORD dwWhileTime = 0;				// ϵͳ����ѭ���ݴ�ʱ��
DWORD dwCurTask = 0;				// ��ǰ��ִ�е�������
DWORD dwRanks = 0;					// ��ǰ�Ƿ����(1�Ƕӳ�, 2�Ƕ�Ա)
DWORD dwRankNum = 0;				// �ӳ���ӵȴ��Ķ�Ա������Ա

HANDLE OpenFunc(DWORD FuncNum)
{
	HANDLE hdThread = NULL;

	switch (FuncNum)
	{
	case OPEN_FUNC_COLLTASK:
		hdThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)HangUp,
			0, 0, NULL);
		break;
	case OPEN_FUNC_INSPECT:
		hdThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)HunterInspect,
			0, 0, NULL);
		return hdThread;
	case OPEB_FUNC_NEWPRESON:
		hdThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ComeNewBady,
			0, 0, NULL);
		break;
	case OPEN_FUNC_AUTOPALYMOST:
		hdThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)AutoPlayMost,
			0, 0, NULL);
		break;
	case OPEN_FUNC_AUTOPALYMOSTA:
		hdThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)PlayPrimary,
			0, 0, NULL);
		break;
	case OPEN_FUNC_AUTOPALYMOSTB:
		hdThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)PlayLower,
			0, 0, NULL);
		break;
	case OPEN_FUNC_AUTOPALYMOSTC:
		hdThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)PlayTeBie,
			0, 0, NULL);
		break;
	case OPEN_FUNC_TRADD:
		hdThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)WareHouse,
			0, 0, NULL);
		break;
	case OPEN_FUNC_TRANSFER:
		hdThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)GoTrade,
			0, 0, NULL);
		break;

	default:
		break;
	}
	dwCurTask = FuncNum;
	return hdThread;
}

void CloseFunc(HANDLE hdThread)
{
	if (!hdThread)
	{
		WinPrintA("[dbg//%s]: ��Ϸ��δִ�й���", playerName);
		return;
	}
	TerminateThread(hdThread, 0);
	CloseHandle(hdThread);
}

void GamePrint(char* str)
{
	CHAR VALE[MAX_PATH] = { 0 };
	SYSTEMTIME time = { 0 };
	GetLocalTime(&time);
	//sprintf(VALE, "[%.2d:%.2d:%.2d] %s", time.wHour, time.wMinute, time.wSecond, str);
	sprintf(VALE, "%s",str);
	//ASM_Print(VALE);
	return;
}

void HangUp()
{
	DWORD dwThreadStat = 1;
	while (1)
	{
		if (dwThreadStat == 1 || dwThreadStat == 0)
		{
			if (!GoShop())									// ǰ���̵귷��
				continue;
			dwThreadStat = 2;
		}

		if (dwThreadStat == 2)
		{
			if (!GoHospi())									// ǰ��ҽԺ
				continue;
			dwThreadStat = 3;
		}
		if (dwThreadStat == 3)
		{
			if (!GoField())									// ǰ���ɼ���
				continue;
			dwThreadStat = 1;
		}

	}
}



// ǰ���̵�
DWORD GoShop()
{
	DeliPoint(3);

	// Ѱ·ȥ�̵�
	if (!GoRect(156, 130))
		return 0;

	if (!GoRect(156, 123))
		return 0;
	// ������Ʒ
	Peddling();
	Sleep(SLEEP_TIME_USUAL);
	WinPrintA("[dbg//%s]: ��ʼ�������������г��������������Ʒ", playerName);
	DiscardGoods(WhiteListID, WHITELIST_MAX);
	Sleep(SLEEP_TIME_USUAL);
	return 1;
}

// ��Ϸ��Ʒ����
void Peddling()
{
	unsigned long addre = 0;			// ������Ʒ����ʼ��ַ
	unsigned char sign = 0;				// ������Ʒ�Ƿ������Ʒ
	unsigned long num = 0;				// ������Ʒ������
	//ASM_RBtnDown(RoleDirec[0]);
	ASM_NPCInteract(NPCRole[0].X, NPCRole[0].Y);
	Sleep(SLEEP_TIME_DELIVE);
	ASM_DoctorReco(3);
	Sleep(SLEEP_TIME_USUAL);
	ASM_OpenShop(2);
	WinPrintA("[dbg//%s]: ��ʼ������Ʒ", playerName);
	Sleep(SLEEP_TIME_DELIVE);

	for (int iStart = 0; iStart < 20; iStart++)
	{
		addre = (ULONG)KnapBase + iStart * 3148;
		sign = ASM_GetAddreByte(addre);
		num = ASM_GetAddreDword(addre + 8);
		if (sign)
		{
			ASM_SellShop(PeddlIndexA[iStart]);
			Sleep(100);
			num = ASM_GetAddreDword(addre + 8);
			if (!num)
				continue;

			ASM_SellShop(PeddlIndexA[iStart]);
			Sleep(100);
		}

	}
	ASM_ReadyOK(1);
	Sleep(SLEEP_TIME_USUAL);
	ASM_DoctorReco(5);
	Sleep(SLEEP_TIME_USUAL);
	return;
}

// ǰ��ҽԺ
DWORD GoHospi()
{
	// ���͵���ҽԺ����Ĵ��͵�
	int i = 0;
	DeliPoint(5);
	// Ѱ·ȥҽԺ
	if (!GoRect(233, 84))
		return 0;
	if (!GoRect(221, 84))
		return 0;
	WinPrintA("[dbg//%s]: ����ҽԺ�ſ�", playerName);
	ASM_RoleWalk(0, 2);
	i = 7;
	while (i--)
	{
		if (ASM_GetRoleCoorX() == 12 && ASM_GetRoleCoorY() == 42)
			break;
		Sleep(SLEEP_TIME_DELIVE);
	}
	if (!GoRect(12, 35))
		return 0;
	if (!GoRect(8, 35))
		return 0;
	if (!GoRect(8, 31))
		return 0;
	// ҽԺ��Ѫ����
	Reply(0);
	// ��ǰ����ֵΪ������,��������޷�ʹ�ü��ָܻ�,ȥҽ�����ָ�
	if(ASM_GetAddreByte((unsigned int)playHelth))
	{
		//for (int i = 0; i < 3; i++)
		//	if (HealthRecov())
		//	{
		//		Reply(0);
		//		return 1;
		//	}

		WinPrintA("[dbg//%s]: ��ǰ����ֵ����", playerName);
		Sleep(1000);
		if (!GoRect(15, 31))
			return 0;
		if (!GoRect(15, 18))
			return 0;
		if (!GoRect(12, 18))
			return 0;
		Reply(1);
	}


	return 1;
}

// ҽ��/��ʿ��Ѫ
void Reply(int Index)
{
	if (Index == 0)
	{
		//ASM_RBtnDown(RoleDirec[0]);
		ASM_NPCInteract(NPCRole[1].X, NPCRole[1].Y);
		Sleep(SLEEP_TIME_DELIVE);
		ASM_DoctorReco(1);
		Sleep(SLEEP_TIME_USUAL);
		ASM_ReadyOK(4);
		Sleep(SLEEP_TIME_USUAL);
		ASM_ReadyOK(1);
		Sleep(SLEEP_TIME_USUAL);
		ASM_DoctorReco(3);
		Sleep(SLEEP_TIME_USUAL);
		ASM_ReadyOK(4);
		Sleep(SLEEP_TIME_USUAL);
		ASM_ReadyOK(1);
		Sleep(SLEEP_TIME_USUAL);
		ASM_DoctorReco(5);
		Sleep(SLEEP_TIME_USUAL);
		ASM_ReadyOK(4);
		Sleep(SLEEP_TIME_USUAL);
		ASM_ReadyOK(1);
	}
	else if (Index == 1)
	{
		ASM_NPCInteract(NPCRole[2].X, NPCRole[2].Y);
		Sleep(SLEEP_TIME_DELIVE );
		ASM_DoctorReco(7);
		Sleep(SLEEP_TIME_USUAL);
		ASM_ReadyOK(4);
		Sleep(SLEEP_TIME_USUAL);
		ASM_ReadyOK(1);
		Sleep(SLEEP_TIME_USUAL);
		ASM_ReadyOK(4);
		Sleep(SLEEP_TIME_USUAL);
	}
	return;
}

// ǰ���ɼ���
DWORD GoField()
{
	int iNum = 0;
	DeliPoint(5);
	// Ѱ·ȥ�ɼ���
	if (!GoRect(234, 88))
		return 0;
	if (!GoRect(278, 88))
		return 0;
	ASM_RoleWalk(2, 4);
	Sleep(SLEEP_TIME_DELIVE);
	while(!(ASM_GetRoleCoorX() == 475 && ASM_GetRoleCoorY(196)))
		Sleep(SLEEP_TIME_DELIVE);

	if (!GoRect(487, 196))
		return 0;
	if (!GoRect(487, 161))
		return 0;
	if (!GoRect(502, 161))
		return 0;

	Sleep(5000);
	while (GetMoster())
		Sleep(SLEEP_TIME_PLAYSTRANG);

	// ��Ʒ�ɼ�
	Collec();
	return 1;
}

// �ɼ���Ʒ
void Collec()
{
	char SkillColle[] = { 0xAC, 0xBC, 0xC2, 0x79, 00, 00 };		// �ɼ���������
	int index = GetSkillIndexByName(SkillColle);
	if (index == -1)
	{
		MessageBoxA(NULL, "û���ڼ��������ҵ����Լ���", playerName, MB_OK);
		return;
	}
	WinPrintA("[dbg//%s]: ���Լ��ܵ��±���%d", playerName, index);
	ASM_UseSkill(index, 0);
	WinPrintA("[dbg//%s]: ��ʼ�ɼ�", playerName, index);
	int iStart = 0;
	Sleep(SLEEP_TIME_USUAL);
	unsigned short sNum = 0;
	unsigned int PlayerMP = 0;
	while (iStart < 20)
	{
		unsigned long addre = (ULONG)KnapBase + iStart * 3148;
		unsigned char sign = ASM_GetAddreByte(addre);
		unsigned long num = ASM_GetAddreDword(addre + 8);
		if (sign)
		{
			iStart++;
			if (iStart > 20)
			{
				WinPrintA("[dbg//%s]: ������Ʒ������\n", playerName, iStart, (int*)addre, sign, num);
				return;
			}
			continue;
		}

		if (sscanf_s(playerMP, "%d", &PlayerMP))
		{
			if (PlayerMP == 0)
			{
				WinPrintA("[dbg//%s]: ħ���ľ�:%s\n", playerName, playerMP);
				GamePrint("ħ���ľ�");
				return;
			}
		}else
			GamePrint("�ɼ�ʱ��⵱ǰħ������");

		if (!(ASM_GetRoleCoorX() == 502 && ASM_GetRoleCoorY() == 161))
		{
			WinPrintA("[dbg//%s]: �ɼ��������,ǰ���ɼ���%d %d\n", playerName, 502, 161);
			GamePrint("�ɼ��������");
			GoRect(502, 161);
			ASM_UseSkill(index, 0);
			Sleep(SLEEP_TIME_USUAL);
		}

		if (ASM_GetAddreByte((unsigned int)SKILL_STACE) != 7 || GetMoster())
		{
			while (GetMoster())
				Sleep(SLEEP_TIME_PLAYSTRANG);

			WinPrintA("[dbg//%s]: �ɼ�״̬�ı�%d,���߲ɼ�����,����ʹ�òɼ�����\n", playerName,
				ASM_GetAddreByte((unsigned int)SKILL_STACE));

			GamePrint("����ʹ�òɼ�����");
			Sleep(SLEEP_TIME_PLAYSTRANG);
			ASM_UseSkill(index, 0);
			Sleep(SLEEP_TIME_USUAL);
		}

		//if (ASM_GetAddreByte((unsigned int)playHelth) && PlayerMP > 40 && sNum < 3)
		//{
		//	HealthRecov();			// ʹ�ûָ���������
		//	sNum++;
		//}


		if(dwFastField)
			*MinTime = GetTickCount64() - 3000;		// ����ϵͳ�����ڿ󷢰���ʱ��

		//WorkOver(index, 0);				// �Լ�����
		Sleep(SLEEP_TIME_USUAL);
	}
	return;
}


// б����
DWORD GoRect(int x, int y)
{
	WinPrintA("[dbg//%s]: Ѱ·��ʼ,ǰ������[%d, %d]", playerName,  x, y);
	unsigned int fault = 0;			// ����·��ʱ��һ���ݴ����
	int curX = 0;					// ��ǰX	
	int curY = 0;					// ��ǰY
	int goBack = 0;					// ��·����
	int GoStep = 0;					// ��·����
	dwWhileTime = GetTickCount64()+20000;
	DWORD dwCurentTime = 0;
	while (1)
	{
		curX = ASM_GetRoleCoorX();
		curY = ASM_GetRoleCoorY();
		dwCurentTime = GetTickCount64();
		//if (RoleHP < 50)
		//	return 0;
		if (dwCurentTime > dwWhileTime)
		{
			if (!fault)
			{
				WinPrintA("[dbg//%s]: ��һ����·��ʱ\n", playerName);
				dwWhileTime = GetTickCount64() + 30000;
				fault++;
				continue;
			}
			WinPrintA("[dbg//%s]: ��·��ʱ2�Σ���������ִ������\n", playerName);
			GamePrint("��·��ʱ,��·���̷���");
			return 0;
		}
		if (curX == x && y == curY)
			break;

		if (curY != y)
		{
			GoStep = abs(curY - y);

			if (curY > y)
				goBack = 0;
			else
				goBack = 4;
		}
		else if (curX != x)
		{
			GoStep = abs(curX - x);

			if (curX > x)
				goBack = 6;
			else
				goBack = 2;
		}
		if (GoStep >=7)
			GoStep = 7;

		ASM_RoleWalk(goBack, GoStep);
		if (GoStep <= 3)
			Sleep(SLEEP_TIME_WALK * GoStep + 100);
		else
			Sleep(SLEEP_TIME_WALK * GoStep);

		if (GetMoster())
		{
			WinPrintA("[dbg//%s]: ��·ʱ��⵽���ڴ��,�ӳٵȴ�\n", playerName);
			Sleep(SLEEP_TIME_INSPECT);
			dwWhileTime = GetTickCount64() + 30000;
			continue;
		}

		if (ASM_GetRoleCoorX() == curX && ASM_GetRoleCoorY() == curY)
		{

			if (GetMoster())
			{
				WinPrintA("[dbg//%s]: ��·ʱ��������\n", playerName);
				dwWhileTime = GetTickCount64() + 1000 * 60 * 2;
				Sleep(SLEEP_TIME_INSPECT);
				while (GetMoster())
				{
					if (GetTickCount64() > dwWhileTime)
					{
						WinPrintA("[dbg//%s]: ������ƿ��ڹ������,���ͻسǻس�\n", playerName);
						ASM_CallHome();
						GamePrint("���ƿ��ڴ�ֽ���,���ͻس�");
						return 0;
					}
					Sleep(SLEEP_TIME_INSPECT);
				}
				continue;
			}

			WinPrintA("[dbg//%s]: ��·�ϰ�����\n", playerName);
			if (goBack + 2 > 7)
				ASM_RoleWalk(goBack + 2 - 8, 1);
			else
				ASM_RoleWalk(goBack + 2, 1);

			Sleep(SLEEP_TIME_WALK);
			ASM_RoleWalk(goBack, 2);
			Sleep(SLEEP_TIME_WALK * 2);

			if (goBack - 2 < 0)
				ASM_RoleWalk(goBack + 8 - 2, 1);
			else
				ASM_RoleWalk(goBack - 2, 1);

			Sleep(SLEEP_TIME_WALK);
		}
		continue;
	}
	return 1;
}

// ˮƽ��
DWORD GoLect(int TarX, int TarY)
{
	WinPrintA("[dbg//%s]: Ѱ·��ʼ,ǰ������[%d, %d]", playerName, TarX, TarY);
	unsigned int fault = 0;			// ����·��ʱ��һ���ݴ����
	int curX = 0;					// ��ǰX	
	int curY = 0;					// ��ǰY
	int goBack = 0;					// ��·����
	int GoStep = 0;					// ��·����
	dwWhileTime = GetTickCount64() + 20000;
	DWORD dwCurentTime = 0;
	while (1)
	{
		curX = ASM_GetRoleCoorX();
		curY = ASM_GetRoleCoorY();
		dwCurentTime = GetTickCount64();
		if (dwCurentTime > dwWhileTime)
		{
			if (!fault)
			{
				WinPrintA("[dbg//%s]: ��һ����·��ʱ\n", playerName);
				dwWhileTime = GetTickCount64() + 30000;
				fault++;
				continue;
			}
			WinPrintA("[dbg//%s]: ��·��ʱ2�Σ���������ִ������\n", playerName);
			GamePrint("��·��ʱ,��·���̷���");
			return 0;
		}
		if (curX == TarX && TarY == curY)
		{
			WinPrintA("[dbg//%s]: ����Ŀ��� %d %d\n", playerName , curX, curY);
			break;
		}

		if (curY != TarY && curX != TarX)
		{
			GoStep = abs(TarY - curY);
			if (curY > TarY)
			{
				
				if (curX > TarX)
					goBack = 7;
				else if (curX < TarX)
					goBack = 1;
			}
			else if(curY < TarY)
			{
				if (curX < TarX)
					goBack = 3;
				if (curX > TarX)
					goBack = 5;
			}
		}
		else
		{
			GoStep = abs(curY - TarY);
			if (curX == TarX && curY > TarY)
				goBack = 0;
			else if (curX == TarX && curY < TarY)
				goBack = 4;
			else if (curX > TarX && curY == TarY)
			{
				goBack = 6;
				GoStep = abs(curX - TarX);
			}
			else
			{
				GoStep = abs(curX - TarX);
				goBack = 2;
			}
				
		}
		if (GoStep >= 7)
			GoStep = 7;

		ASM_RoleWalk(goBack, GoStep);
		if(GoStep<=3)
			Sleep(SLEEP_TIME_WALK * GoStep + 100);
		else
			Sleep(SLEEP_TIME_WALK* GoStep);

		if (GetMoster())
		{
			WinPrintA("[dbg//%s]: ��·ʱ��⵽���ڴ��,�ӳٵȴ�\n", playerName);
			Sleep(SLEEP_TIME_INSPECT);
			dwWhileTime = GetTickCount64() + 30000;
			continue;
		}

		if (ASM_GetRoleCoorX() == curX && ASM_GetRoleCoorY() == curY)
		{
			if (GetMoster())
			{
				WinPrintA("[dbg//%s]: ��·ʱ��������\n", playerName);
				dwWhileTime = GetTickCount64() + 1000 * 60 * 2;
				Sleep(SLEEP_TIME_INSPECT);
				while (GetMoster())
				{
					if (GetTickCount64() > dwWhileTime)
					{
						WinPrintA("[dbg//%s]: ������ƿ��ڹ������,���ͻسǻس�\n", playerName);
						ASM_CallHome();
						GamePrint("���ƿ��ڴ�ֽ���,���ͻس�");
						return 0;
					}
					Sleep(SLEEP_TIME_INSPECT);
				}
				continue;
			}
			WinPrintA("[dbg//%s]: ��·�ϰ�����\n", playerName);
			if(goBack +2 > 7)
				ASM_RoleWalk(goBack+2 -8, 1);
			else
				ASM_RoleWalk(goBack + 2, 1);

			Sleep(SLEEP_TIME_WALK);
			ASM_RoleWalk(goBack, 2);
			Sleep(SLEEP_TIME_WALK*2);

			if(goBack -2 <0)
				ASM_RoleWalk(goBack + 8 - 2 , 1);
			else
				ASM_RoleWalk(goBack - 2, 1);

			Sleep(SLEEP_TIME_WALK);
		}
		continue;
	}

	return 1;
}

void DeliPoint(int Index)
{
	WinPrintA("[dbg//%s]: ǰ��[%d, %d]���͵�,���Ϳ�ʼ", playerName,
		aircraft[Index][0], aircraft[Index][1]);
	int X = 0;
	int Y = 0;

	// �ж�Ҫȥ���������������±����2
	if (Index > 2)
		while (1)
		{
			Sleep(SLEEP_TIME_DELIVE);
			X = ASM_GetRoleCoorX();
			Y = ASM_GetRoleCoorY();
			if (X == aircraft[Index][0] && Y == aircraft[Index][1])
				break;
			if (X == aircraft[5][0] && Y == aircraft[5][1])
			{
				ASM_NPCInteract(aircraft[5][2], aircraft[5][3]);
				continue;
			}
			if (X == aircraft[4][0] && Y == aircraft[4][1])
			{
				ASM_NPCInteract(aircraft[4][2], aircraft[4][3]);
				continue;
			}
			if (X == aircraft[3][0] && Y == aircraft[3][1])
			{
				ASM_NPCInteract(aircraft[3][2], aircraft[3][3]);
				continue;
			}
			ASM_CallHome();
			continue;
		}
	else
		while (1)
		{
			Sleep(SLEEP_TIME_DELIVE);
			X = ASM_GetRoleCoorX();
			Y = ASM_GetRoleCoorY();
			if (X == aircraft[Index][0] && Y == aircraft[Index][1])
				break;
			if (X == aircraft[0][0] && Y == aircraft[0][1])
			{
				ASM_NPCInteract(aircraft[0][2], aircraft[0][3]);
				continue;
			}
			if (X == aircraft[1][0] && Y == aircraft[1][1])
			{
				ASM_NPCInteract(aircraft[1][2], aircraft[1][3]);
				continue;
			}
			if (X == aircraft[2][0] && Y == aircraft[2][1])
			{
				ASM_NPCInteract(aircraft[2][2], aircraft[2][3]);
				continue;
			}
			ASM_CallHome();
			continue;
		}
	Sleep(SLEEP_TIME_USUAL);
	WinPrintA("[dbg//%s]: ������ɵ�ǰλ��[%d %d]", playerName,
		X, Y);
}



void HunterInspect(void)
{
	WinPrintA("[dbg//%s]:  %s  %s [%s]\n", playerName, AppUser, AppPass,
		 AppServerIP);
	WinPrintA("[dbg//%s �������]: ��Ϸ��̨������̿���,�������,���ּ��\n", playerName);
	HANDLE PuseThread = 0;
	char arr[10] = { 0 };
	unsigned int PlayerMP =  0 ;
	unsigned int PlayerHP =  0 ;
	while (1)
	{	
		if (ASM_GetAddreDword((unsigned int)attackStatus) == 1 || ASM_GetAddreDword((unsigned int)attackStatus) == 4)
		{
			WinPrintA("[dbg//%s�������]:  ��������!\n", playerName);
			PuseThread = FuncThread;
			if (PuseThread)
			{
				SuspendThread(PuseThread);
			}
				

			Battle();

			WinPrintA("[dbg//%s�������]:  �������,����ȫ������\n", playerName);
			Sleep(SLEEP_TIME_USUAL);

			SetRoleAttr();
			dwWhileTime = GetTickCount64() + 20000;
			sscanf_s(playerHP, "%d", &PlayerHP);
			PuseThread = FuncThread;

			if (dwRanks == 1)
			{
				Sleep(SLEEP_TIME_INSPECT);
				if (GetGroupNum() < dwRankNum)
				{
					WinPrintA("[dbg//%s�������]: �ӳ���Ӵ������,��ǰ��Ա����%dС��Ԥ���Ա����%d\n", playerName,
						GetGroupNum(), dwRankNum);
					CloseFunc(PuseThread);
					Sleep(SLEEP_TIME_INSPECT);
					FuncThread = OpenFunc(dwCurTask);
					continue;
				}
			}
			if (dwCurTask == OPEB_FUNC_NEWPRESON || PlayerHP > 100)
			{
				if (PuseThread)
					ResumeThread(PuseThread);
				continue;
			}

			WinPrintA("[dbg//%s�������]: ս������,����Ѫ������\n", playerName);

			if (PuseThread)
			{
				CloseFunc(PuseThread);
				Sleep(SLEEP_TIME_INSPECT);
				FuncThread = OpenFunc(dwCurTask);
			}
			continue;
		}

		//if (GetAddreByte((unsigned int)playHelth))
		//{

		//	sscanf(playerMP, "%d", &PlayerMP);
		//	if (PlayerMP > 40)
		//	{
		//		WinPrintA("[dbg//%s�������]:  ���ﵱǰ���ǽ���״̬,���Լ��ָܻ�\n", playerName);
		//		PuseThread = FuncThread;
		//		if (PuseThread)
		//			SuspendThread(PuseThread);

		//		HealthRecov();			// ʹ�ûָ���������


		//		if (PuseThread)
		//			ResumeThread(PuseThread);
		//			
		//	}else
		//		WinPrintA("[dbg//%s�������]:  ���ﵱǰ���ǽ���״̬,ħ������\n", playerName);
		//}
		Sleep(SLEEP_TIME_INSPECT);
	}
}



void Battle()
{
	Sleep(SLEEP_TIME_DELIVE);
	DWORD dwRoleHP = 0;
	while (1)
	{
		sscanf_s(playerHP, "%d", &dwRoleHP);
		//if (dwRoleHP < 50)
		//{
		//	ASM_CallHome();
		//	return;
		//}
			
		if (*attackStatus == 1)
		{
			WinPrintA("[dbg//%s�������]:  ����غ�\n", playerName);
			ASM_playerAttack(GetMoster());
			dwWhileTime = GetTickCount64() + 20000;
			if (dwAutoFastMost)
			{
				*MostJMP = 1;
				Sleep(SLEEP_TIME_PLAYSTRANG);
			}
				
		}
		if (*attackStatus == 4)
		{
			WinPrintA("[dbg//%s�������]:  ����غ�\n", playerName);
			ASM_petAttack(GetMoster(), 0);
			dwWhileTime = GetTickCount64() + 20000;
			if (dwAutoFastMost)
			{
				*MostJMP = 1;
				Sleep(SLEEP_TIME_PLAYSTRANG);
			}
		}

		if (!GetMoster())
		{
			WinPrintA("[dbg//%s�������]:  GetMosterû�м�⵽��\n", playerName);
			return;
		}
		Sleep(SLEEP_TIME_USUAL);

	}


}


void SetRoleAttr()
{
	DWORD dwIndex = 0;  // ���Ե�

	// �ж��������Ե�,���ڵ���2�ӵ�(����1,����1)
	if (ASM_GetAddreDword((unsigned int)playAttribut))
	{
		GamePrint("�����Զ��ӵ�");
		dwIndex = ASM_GetAddreDword((unsigned int)playAttribut);
		for (int i = 0; i < dwIndex; i++)
		{
			//ASM_RoleAddAttr(0);
			//Sleep(100);
			//ASM_RoleAddAttr(1);
			//Sleep(100);
			//ASM_RoleAddAttr(4);
			//Sleep(100);
			ASM_RoleAddAttr(4);
			Sleep(100);
		}
	}
	if (ASM_GetAddreDword((unsigned int)playPetsAttr))
	{
		dwIndex = ASM_GetAddreDword((unsigned int)playPetsAttr);
		GamePrint("�����Զ��ӵ�");
		for (int i = 0; i < dwIndex; i++)
		{
			ASM_AddPetsAttr(1);
			Sleep(100);
		}
	}
	return 0;
}


DWORD GetMoster()
{
	DWORD dwIndex = 0;
	for (int i = 10; i < 26; i++)
		if (ASM_GetAddreDword((unsigned int )battleArrBase + i * 4))
			dwIndex =  i;

	return dwIndex;
}

int GetGoodsInex(int GoodsType, int GoodsId)
{
	unsigned long addre = 0;
	for (int i = 0; i < 20; i++)
	{
		addre = (ULONG)KnapBase + i * 3148;
		if (ASM_GetAddreDword(addre + 4) == GoodsType && ASM_GetAddreDword(addre + 0xc) == GoodsId)
			return 8 + i;
	}

	return -1;
}


// ������Ʒ���ֲ�����Ʒ���������ﱳ����λ��,û�з���-1	
int GetRoleGoodByName(char* GoodsName)
{
	unsigned int addre = 0;
	for (int i = 8; i < 28; i++)
	{
		addre = (unsigned int)RoleKnapBase + i * 0xc5c+2;
		if (!strcmp(GoodsName, addre))
		{
			WinPrintA("[dbg//%s]: ��Ʒ[%s] �ڱ�����λ��%d \n", playerName, GoodsName, i);
			return i;
		}
			
	}
	WinPrintA("[dbg//%s]: û���ҵ���Ʒ[%s]\n", playerName, GoodsName);
	return -1;
}

int GetSkillIndexByName(char* Skill_NAME)
{
	DWORD Index = 0;
	for (int i = 0; i < 10; i++)
	{
		Index = ASM_GetAddreWord(SKILL_INDEX_BASE + i * 8);
		if (!strcmp((ULONG)SKILL_BASE + Index * 0x4c4c, Skill_NAME))
		{
			WinPrintA("[dbg//%s]: ��ż��ܵĵ�ַ[%x] ����λ��%x\n", playerName,
				SKILL_INDEX_BASE + i * 8,
				Index);
			return Index;
		}

	}		
	return -1;
}

int GetSkillNumByName(char* Skill_NAME)
{
	DWORD Index = 0;
	for (int i = 0; i < 10; i++)
	{
		Index = ASM_GetAddreWord(SKILL_INDEX_BASE + i * 8);
		DWORD* dwNmae = (DWORD*)Skill_NAME;
		if (!strcmp((unsigned int)SKILL_BASE + Index * 0x4c4c, Skill_NAME))
		{
			if (ASM_GetAddreDword((unsigned int)SKILL_BASE + Index * 0x4c4c + 0X1f8) == *dwNmae)
				return 3;
			if (ASM_GetAddreDword((unsigned int)SKILL_BASE + Index * 0x4c4c + 0x16a) == *dwNmae)
				return 2;
			if (ASM_GetAddreDword((unsigned int)SKILL_BASE + Index * 0x4c4c + 0xd0) == *dwNmae)
				return 1;
			if (ASM_GetAddreDword((unsigned int)SKILL_BASE + Index * 0x4c4c + 0x3c) == *dwNmae)
				return 0;
		}
	}

	return -1;
}

// ʹ�ü��ָܻ�����, �ɹ�����1 ʧ�ܷ���0
DWORD HealthRecov()
{
	char SkillName[] = { 0xAA, 0x76, 0xC0, 0xF8, 00, 00 };
	DWORD dwHealth = ASM_GetAddreByte((ULONG)playHelth);
	if (!dwHealth)
	{
		WinPrintA("[dbg//%s]: ���ﵱǰΪ����\n", playerName);
		return 1;
	}
	DWORD SkillIndex = GetSkillIndexByName(SkillName);
	if (SkillIndex == -1)
	{
		WinPrintA("[dbg//%s]: ���ﵱǰû�лظ������ļ���\n", playerName);
		return 0;
	}
	DWORD SkillNum = GetSkillNumByName(SKILL_TREAT);
	WinPrintA("[dbg//%s]: ����ָ�������������Ϊ%d ��ǰ����ӵ��%d���ȼ�\n", playerName,SkillIndex,SkillNum+1);
	//if (dwHealth / 25 > SkillNum)
	//{
	//	WinPrintA("[dbg//%s]: ���ﵱǰ�����޷��ָ���ô���ص�����\n", playerName);
	//	return 0;
	//}
	ASM_useSkillRecov(SkillNum);
	Sleep(SLEEP_TIME_USUAL);
	WinPrintA("[dbg//%s]: ����ָ�, ѡ���������\n", playerName);
	ASM_useRecovItemOne(0);
	Sleep(SLEEP_TIME_USUAL);
	WinPrintA("[dbg//%s]: ����ָ�, ѡ��ָ���ɫ\n", playerName);
	ASM_useRecovItemTwo(0);
	Sleep(SLEEP_TIME_DELIVE);
	if (!ASM_GetAddreByte((unsigned int)playHelth))
	{
		WinPrintA("[dbg//%s]: ����ɹ��ָ�Ϊ����\n", playerName);
		return 1;
	}
	WinPrintA("[dbg//%s]: ����ָ�ʧ��\n", playerName);
	//ASM_CloseRecov();			// �رջָ���������
	return 0;
}


void DiscardGoods(int dwWhiteList[][2], int Num)
{
	unsigned int addre = 0;
	unsigned char sign = 0;
	int uiGoodID = 0;
	int uiGoodType = 0;
	for (int i = 0; i < 20; i++)
	{
tab:
		addre = (ULONG)KnapBase + i * 3148;
		sign = ASM_GetAddreByte(addre);
		if (sign)
		{
			uiGoodType = ASM_GetAddreDword(addre + 4);
			uiGoodID = ASM_GetAddreDword(addre + 0xc);
			for (int j = 0; j < Num; j++)
				if ( uiGoodType == dwWhiteList[j][0] || uiGoodID == dwWhiteList[j][1])
				{
					i++;
					goto tab;
				}
			ASM_DeleteGoods(i + 1);
			Sleep(100);
		}
	}
}


void CreateNewRole(void)
{
	WinPrintA("[dbg//%s]: �½�ɫ������ʼ\n", playerName);
	DWORD dwThreadStat = 1;
	while (1)
	{
		if (dwThreadStat == 1 || dwThreadStat == 0)
		{
			if (!GoNewPerson())
			{
				ASM_CallHome();
				Sleep(SLEEP_TIME_DELIVE);
				continue;
			}
			dwThreadStat = 2;
		}
		if (dwThreadStat == 2)
		{
			dwThreadStat = GoNewPersonOver();
			if (dwThreadStat == -1)
			{
				dwThreadStat = 1;
				continue;
			}
			else if (dwThreadStat == 1)
			{
				WinPrintA("[dbg//%s]: �½�ɫ�����������̳ɹ�\n", playerName);
				return 0;
			}
		}

	}
}


DWORD GoNewPerson(void)
{
	char GoodsName[] = { 0xA6,0xBA, 0xAA, 0xCC, 0xAA, 0xBA, 0xA7 ,0xD9 ,0xAB ,0xFC, 0x0, 0 };  // ���߽�ָ
	DWORD dwWhileNum = 0;
	DWORD dwIndex = 0;
	GoRect(18, 6);
	Sleep(SLEEP_TIME_WALK);
	ASM_NPCInteract(19, 6);
	Sleep(SLEEP_TIME_DELIVE);
	ASM_ReadyOK(4);
	Sleep(SLEEP_TIME_USUAL);
	ASM_ReadyOK(0x20);
	Sleep(SLEEP_TIME_USUAL);
	ASM_ReadyOK(0x20);
	Sleep(SLEEP_TIME_USUAL);
	ASM_ReadyOK(0x20);
	Sleep(SLEEP_TIME_USUAL);
	ASM_ReadyOK(1);
	Sleep(SLEEP_TIME_DELIVE);
	dwWhileNum = 0;
	while (1)
	{
		if ((ASM_GetRoleCoorX() == 44 && ASM_GetRoleCoorY() == 10))
			break;

		if (dwWhileNum > 20)
			return 0;
		ASM_ReadyOK(0x20);
		dwWhileNum++;
		Sleep(SLEEP_TIME_USUAL);
	}

	ASM_WearItem(8, 2);
	Sleep(SLEEP_TIME_DELIVE);
	if (!GoRect(9, 5))
		return 0;
	if (!GoRect(9, 28))
		return 0;
	if (!GoRect(23, 28))
		return 0;
	if (!GoRect(23, 20))
		return 0;
	ASM_RoleWalk(0, 1);
	Sleep(SLEEP_TIME_DELIVE);
	if (!GoRect(30, 50))
		return 0;
	if (!GoRect(21, 52))
		return 0;
	if (!GoRect(52, 37))
		return 0;
	if (!GoRect(53, 2))
		return 0;
	/* ��ָ*/
	ASM_NPCInteract(54, 2);
	Sleep(SLEEP_TIME_USUAL);
	ASM_ReadyOK(4);
	Sleep(SLEEP_TIME_USUAL);
	ASM_ReadyOK(1);
	Sleep(2000);
	dwWhileNum = 0;
	while (1)
	{
		dwIndex = GetRoleGoodByName(&GoodsName);
		if (dwIndex != -1)
			break;
			
		if (dwWhileNum > 20)
			return 0;
		dwWhileNum++;
		ASM_ReadyOK(4);
		Sleep(SLEEP_TIME_USUAL);
	}
	if (!GoRect(52, 25))
		return 0;
	if (!GoRect(21, 37))
		return 0;
	if (!GoRect(30, 52))
		return 0;
	if (!GoRect(31, 49))
		return 0;
	ASM_RoleWalk(0, 1);
	Sleep(SLEEP_TIME_DELIVE);
	if (!GoRect(8, 27))
		return 0;
	if (!GoRect(43, 4))
		return 0;
	if (!GoRect(44, 14))
		return 0;
	ASM_RoleWalk(4, 2);
	Sleep(SLEEP_TIME_DELIVE);
	if (!GoRect(18, 16))
		return 0;
	if (!GoRect(18, 6))
		return 0;
	/* �ڶ��ζԻ�*/
	ASM_NPCInteract(19, 6);
	Sleep(SLEEP_TIME_DELIVE);
	ASM_ReadyOK(0x20);
	Sleep(SLEEP_TIME_USUAL);
	ASM_ReadyOK(0x20);
	Sleep(SLEEP_TIME_USUAL);
	ASM_ReadyOK(0x20);
	Sleep(SLEEP_TIME_DELIVE);
	ASM_ReadyOK(1);
	Sleep(SLEEP_TIME_DELIVE);
	UINT Index = 0;
	while (1)
	{
		if (ASM_GetRoleCoorX() == 9 && ASM_GetRoleCoorY() == 17)
			break;
		if (Index > 20)
			return 0;

		Index++;
		Sleep(SLEEP_TIME_DELIVE);
	}
	return 1;
}

DWORD GoNewPersonOver(void)
{
	char GoodsName[] = { 0xBD, 0xE0, 0xBD, 0xE7, 0xAA, 0xAC, 0x00,0 }; //�������ʹ�״
	char GoodsClothes[] = { 0xB0, 0xEA, 0xA5,  0xC1, 0xB3, 0x54, 0x00, 0xD9, 0xAB,  0xFC, 00,0 }; //ƽ����
	char GoodsShoes[] = { 0xB0, 0xEA, 0xA5, 0xC1, 0xB9, 0x75, 00, 00 }; //ƽ��Ь
	DWORD dwWhileNum = 0;
	DWORD dwIndex = 0;
	/* ����*/
	if (!GoRect(7, 4))
		return 0;
	ASM_NPCInteract(7, 3);
	Sleep(SLEEP_TIME_USUAL);
	ASM_ReadyOK(0x20);
	Sleep(SLEEP_TIME_DELIVE);
	while (1)
	{
		dwIndex = GetRoleGoodByName(&GoodsName);
		if (dwIndex != -1)
			break;
		if (dwWhileNum > 20)
			return 0;
		dwWhileNum++;
		WinPrintA("[dbg//%s]: �½�ɫ����û���ҵ��������ʹ�״ Index=%x  Num = %d\n", playerName, dwIndex, dwWhileNum);
		ASM_ReadyOK(0x20);
		Sleep(SLEEP_TIME_USUAL);

	}
	if (!GoRect(9, 14))
		return 0;
	if (!GoRect(9, 18))
		return 0;
	ASM_RoleWalk(4, 2);
	Sleep(SLEEP_TIME_DELIVE);
	if (!GoRect(50, 55))
		return 0;
	if (!GoRect(51, 78))
		return 0;
	ASM_NPCInteract(51, 79);
	Sleep(SLEEP_TIME_USUAL);
	ASM_ReadyOK(1);
	Sleep(SLEEP_TIME_DELIVE);
	dwWhileNum = 0;
	while (1)
	{
		dwIndex = GetRoleGoodByName(&GoodsClothes);
		if (dwIndex != -1)
			break;
		if (dwWhileNum > 20)
			return 0;
		dwWhileNum++;
		WinPrintA("[dbg//%s]: �½�ɫ����û���ҵ��·�[%s] Index=%x  Num = %d\n", playerName, GoodsName, dwIndex, dwWhileNum);
		ASM_ReadyOK(0x20);
		Sleep(SLEEP_TIME_USUAL);
	}
	ASM_WearItem(dwIndex, 1);
	dwIndex = GetRoleGoodByName(&GoodsShoes);
	if (dwIndex != -1)
		ASM_WearItem(dwIndex, 4);
	WinPrintA("[dbg//%s]: �����ս\n", playerName);
	ASM_ComeBady(2);
	Sleep(SLEEP_TIME_DELIVE);
	DeliPoint(5);
	return 1;
}

void AutoPlayMost(void)
{
	DWORD dwThreadStat = 1;
	int roleLv = 0;					// ���ﵱǰ�ȼ�
	while (1)
	{
		roleLv = ASM_GetAddreDword((unsigned int)playerLV);
		if (dwThreadStat == 1 || dwThreadStat == 0)
		{
			if (!GoShop())									// ǰ���̵귷��
				continue;
			dwThreadStat = 2;
		}

		if (dwThreadStat == 2)
		{
			if (!GoHospi())									// ǰ��ҽԺ
				continue;
			dwThreadStat = 3;
		}

		if (dwThreadStat == 3)
		{
			if (ASM_GetAddreDword((unsigned int)playWorkExp))		// �жϵ�ǰʣ��˫������ʱ��
			{
				if (!TheWorkTime())
					continue;
			}

			if (dwRanks && dwRankNum)
			{
				if (!GoGroup())
				{
					dwThreadStat = 1;
					continue;
				}
			}
			
			WinPrintA("[dbg//%s]: ��ʼ������, ��ǰ�ȼ�%d\n", playerName, roleLv);
			if (roleLv <= 7 && GetGroupNum() ==0)		// ���˵ȼ�С��7���Ҷ��鲢��û�����
				PlayNovic();
			else if (roleLv < 15 && GetGroupNum() <=2) // ���˵ȼ�С��15���� �����Ա����С�ڵ���2
				PlayPrimary();
			else if (roleLv < 20 && GetGroupNum()<=5)  // ���˵ȼ�С��15���� �����Ա����С�ڵ���5
				PlayLower();
			else
				PlayNovic();							// Ĭ�ϴ�ֵص�

			dwThreadStat = 1;
			continue;
		}
	}

}


DWORD PlayNovic(void)
{
	DeliPoint(2);
	if (!GoRect(255, 88))
		return 0;
	if (!GoRect(279, 88))
		return 0;
	ASM_RoleWalk(2, 2);
	Sleep(SLEEP_TIME_DELIVE);
	if (!GoRect(479, 173))
		return 0;
	if (!GoRect(489, 157))
		return 0;
	if (!GoRect(490, 150))
		return 0;
	if (!GoRect(492, 144))
		return 0;
	while (1)
	{
		if (!GoRect(496, 141))
			return 0;
		if (!GoRect(490, 141))
			return 0;
	}
}


DWORD PlayPrimary(void)
{
	DeliPoint(2);
	if (!GoRect(255, 88))
		return 0;
	if (!GoRect(279, 88))
		return 0;
	ASM_RoleWalk(2, 2);
	Sleep(SLEEP_TIME_DELIVE);
	if (!GoRect(490, 196))
		return 0;
	if (!GoRect(502, 207))
		return 0;
	if (!GoRect(515, 227))
		return 0;
	while (1)
	{
		if (!GoRect(510, 237))
			return 0;
		if (!GoRect(518, 256))
			return 0;
		if (!GoRect(496, 268))
			return 0;
		if (!GoRect(474, 275))
			return 0;
		if (!GoRect(485, 292))
			return 0;
		if (!GoRect(462, 284))
			return 0;
		if (!GoRect(428, 275))
			return 0;
		if (!GoRect(455, 259))
			return 0;
		if (!GoRect(487, 245))
			return 0;
		if (!GoRect(508, 236))
			return 0;
		if (!GoRect(515, 227))
			return 0;
	}
}

DWORD PlayLower(void)
{
	DeliPoint(2);
	if (!GoRect(255, 88))
		return 0;
	if (!GoRect(278, 88))
		return 0;
	ASM_RoleWalk(2, 4);
	Sleep(SLEEP_TIME_DELIVE);
	if (!GoRect(489, 196))
		return 0;
	if (!GoRect(499, 203))
		return 0;
	if (!GoRect(508, 225))
		return 0;
	if (!GoRect(528, 232))
		return 0;
	if (!GoRect(571, 234))
		return 0;
	if (!GoRect(577, 242))
		return 0;
	if (!GoRect(574, 255))
		return 0;
	if (!GoRect(598, 268))
		return 0;

	if (!GoRect(598, 263))
		return 0;
	while (1)
	{
		if (!GoLect(596, 261))
			return 0;
		if (!GoLect(593, 264))
			return 0;
		if (!GoLect(600, 271))
			return 0;
		if (!GoLect(603, 268))
			return 0;
	}
}

// ���˴���
DWORD PlayTeBie(void)
{
	if (!GoRect(685, 338))
		return 0;
	if (!GoLect(693, 330))
		return 0;
	if (!GoLect(655, 292))
		return 0;
	if (!GoRect(646, 292))
		return 0;
	if (!GoLect(649, 289))
		return 0;

	while (1)
	{
		if (!GoRect(653, 283))
			return 0;
		if (!GoLect(671, 265))
			return 0;

		if (!GoLect(665, 259))
			return 0;
		if (!GoLect(680, 244))
			return 0;
		if (!GoRect(669, 231))
			return 0;
		if (!GoRect(650, 220))
			return 0;
		if (!GoRect(633, 218))
			return 0;
		if (!GoLect(625, 226))
			return 0;
		if (!GoLect(618, 226))
			return 0;
		if (!GoLect(615, 223))
			return 0;
		if (!GoRect(611, 223))
			return 0;
		if (!GoLect(603, 231))
			return 0;
		if (!GoLect(557, 231))
			return 0;
		if (!GoLect(585, 241))
			return 0;
		if (!GoLect(604, 260))
			return 0;
		if (!GoRect(614, 260))
			return 0;
		if (!GoLect(623, 269))
			return 0;
		if (!GoRect(638, 281))
			return 0;
		if (!GoRect(649, 289))
			return 0;
	}
}

DWORD SetSocial(DWORD StanceNUM)
{
	switch (StanceNUM)
	{
	case 3:
	{
		WinPrintA("[dbg//%s]: ����/�ر��������\n", playerName);
		ASM_IsAddArmy();
		break;
	}
	case 5:
	{
		WinPrintA("[dbg//%s]: ����/�ر�������\n", playerName);
		ASM_IsTrade();
		break;
	}
	default:
		break;
	}
	return 0;
}

void WareHouse(void)
{
	UINT Index = 0;
	WCHAR arr[20] = { 0 };
	GoWare();
	ASM_RoleWalk(2, 1);
	if (!(ASM_GetAddreDword((unsigned int)IsSocialState) & 0x20))
		SetSocial(5);
	while (1)
	{

		if (ASM_GetAddreDword((unsigned int)CurTradeState) & 0x80)
		{
			WinPrintA("[dbg//%s]: �һ��ֿ�ż�⵽����������\n", playerName);
			ASM_TradeGoods(0, &arr);
			WinPrintA("[dbg//%s]: �һ��ֿ�ŵȴ��Է�ȷ�Ͻ�����Ʒ\n", playerName);
			Index = 10;
			while (1)
			{
				Sleep(SLEEP_TIME_USUAL);
				if (Index > 10)
				{
					WinPrintA("[dbg//%s]: �һ��ֿ�Ž���ʧ��,�ȴ��Է�ȷ�ϳ�ʱ\n", playerName);
					break;
				}
				if (!ASM_GetAddreDword(CurTradeCfirm))
				{
					Index++;
					Sleep(SLEEP_TIME_USUAL);
					continue;
				}
				ASM_TradeOK();
				WinPrintA("[dbg//%s]: �һ��ֿ�Ž��׳ɹ� \n", playerName);
				Sleep(5000);
				break;
			}
		}
		Sleep(SLEEP_TIME_INSPECT);
	}
}

DWORD GoWare(void)
{
	DeliPoint(4);
	if (!GoLect(61, 134))
		return 0;
	if (!GoRect(65, 136))
		return 0;
	ASM_RoleWalk(2, 1);
	Sleep(SLEEP_TIME_USUAL);

	return 1;
}

DWORD GoTrade(void)
{
	DWORD Index = 2;
	DWORD dwThreadStat = 0;
	while (1)
	{
		if (dwThreadStat == 0 || dwThreadStat == 1)
		{
			if (!GoHospi())
				continue;

			dwThreadStat = 2;
		}

		if (dwThreadStat == 2)
		{
			WinPrintA("[dbg//%s]: ǰ�����׵ص� \n", playerName);
			if (!GoWare())
				continue;
			dwThreadStat = 3;
		}

		if (dwThreadStat == 3)
		{
			if (GoldTransfer() || Index== 0)
				return 0;
			dwThreadStat = 2;
			Index--;
			WinPrintA("[dbg//%s]: ����ǰ�����׵ص� \n", playerName);
			continue;
		}
	}
}
DWORD GoldTransfer(void)
{
	UINT Index = 0;			// �м������׶���
	UINT CurMoney = 0;		// ��ǰӵ�н��
	WCHAR chBuf[20] = { 0 };
	ASM_Trade();
	Sleep(SLEEP_TIME_DELIVE);
	while (ASM_GetAddreByte((unsigned int)TradeRole + Index * 10))
	{
		//if ((char*)((unsigned int)TradeRole + Index * 10) == "abce")
			//break;
		Index++;
	}
	if (Index > 2 || Index == 0)
	{
		WinPrintA("[dbg//%s]: û���ҵ����׶�����߽��׶��󳬹����� \n", playerName);
		ASM_CallHome();
		return 0;
	}

	ASM_TradeChoice(Index - 1);
	WinPrintA("[dbg//%s]: ���ԺͲֿ�Ž��н��� \n", playerName);
	Sleep(SLEEP_TIME_DELIVE);
	while (1)
	{
		if (ASM_GetAddreByte((UINT)CurTradeState) & 0x80)
		{
			CurMoney = ASM_GetAddreDword((UINT)playMoney);
			ASM_TradeGoods(CurMoney, &chBuf);
			WinPrintA("[dbg//%s]: ���׽��������ȷ�� %d\n", playerName, CurMoney);
			Sleep(SLEEP_TIME_USUAL);
			Index = 0;
			while (1)
			{
				if (ASM_GetAddreDword(CurTradeCfirm))
				{
					ASM_TradeOK(); 
					WinPrintA("[dbg//%s]: ���ת�Ƴɹ� \n", playerName);
					Sleep(SLEEP_TIME_DELIVE * 2);
					DeliPoint(5);
					return 1;
				}
				if (Index > 10)
					return 0;
				Sleep(SLEEP_TIME_USUAL);
				Index++;
				continue;
			}
		}
			// Index�ظ����������ƴ���,����������δ�򿪽��׿�ֱ��break
		if (Index > 10)
		{
			DeliPoint(5);
			return 0;
		}
				
		Index++;
	}
}

DWORD GoGroup()
{
	DWORD dwWaitTime = 0;
	DWORD dwCurRankNum = 0;			// ��ǰ�����Ա
	DeliPoint(1);
	if (!GoRect(60, 81))
		return 0;
	ASM_RoleWalk(6, 1);
	Sleep(SLEEP_TIME_USUAL);
	dwWaitTime = GetTickCount64() + 1000 * 60 * 2;
	if (dwRanks == 1)
	{
		ASM_RoleWalk(6,1);
		if (!(ASM_GetAddreDword((unsigned int)IsSocialState) & 0x1))
			SetSocial(3);
		while (1)
		{
			dwCurRankNum = GetGroupNum();
			WinPrintA("[dbg//%s]: �ӳ��ȴ��������%d, ��ǰ�������� %d\n", playerName, dwRankNum, dwCurRankNum);
			if (dwCurRankNum >= dwRankNum)
				break;
			Sleep(SLEEP_TIME_INSPECT);
		}
		if (!GoRect(63, 79))
			return 0;

		return 1;
	}
	if (dwRanks == 2)
	{
		ASM_AddArmy();
		Sleep(SLEEP_TIME_PLAYSTRANG);
		while (dwWaitTime > GetTickCount64())
		{
			if (!GetGroupNum())
			{
				WinPrintA("[dbg//%s]:��Ա���ʧ�ܳ����ٴ����\n", playerName);
				ASM_AddArmy();
				Sleep(1000 * 15);
				continue;
			}
			while (GetGroupNum())
			{
				Sleep(SLEEP_TIME_PLAYSTRANG);
			}
			WinPrintA("[dbg//%s]:��Ա��Ӵ�ֹ���, ��ǰ�����ɢ\n", playerName);
			return 0;
		}
		if (!GoRect(63, 79))
			return 0;
		return 1;
	}
	return 0;
}


DWORD GetGroupNum()
{
	DWORD GroupNum = 0;
	for (int i = 0; i < 5; i++)
	{
		if (ASM_GetAddreDword((unsigned int)CurRankBase + i * 0x30))
			GroupNum++;
	}
	return GroupNum;
}


DWORD ComeNewBady()
{
	DWORD dwThreadStat = 0;
	while (1)
	{
		ASM_CallHome();
		Sleep(SLEEP_TIME_DELIVE);
		if (ASM_GetRoleCoorX() == 15 && ASM_GetRoleCoorY() == 6)
		{
			WinPrintA("[dbg//%s]: �½�ɫ������ʼ\n", playerName);
			CreateNewRole();
		}
			
		if (dwThreadStat == 1 || dwThreadStat == 0)
		{
			dwCurTask = OPEN_FUNC_AUTOPALYMOST;
			WinPrintA("[dbg//%s]: �ж��½�ɫ�����ѽ���,��ʼ�һ����\n", playerName);
			AutoPlayMost();
		}
		continue;
		// ���ֹ���-->�Զ����
	}

}

DWORD TheWorkTime(void)
{
	DWORD dwWhileNum = 0;
	DeliPoint(1);
	if (!GoRect(65, 79))
		return 0;
	if (!GoRect(73, 65))
		return 0;
	if (!GoRect(73, 62))
		return 0;
	ASM_RoleWalk(0, 3);
	Sleep(SLEEP_TIME_DELIVE);
	dwWhileNum =0;
	while (1)
	{
		if (ASM_GetRoleCoorX() == 9 && ASM_GetRoleCoorY() == 24)
			break;
		dwWhileNum++;
		if (dwWhileNum > 20)
			return 0;
		Sleep(SLEEP_TIME_USUAL);
	}
	if (!GoRect(7, 18))
		return 0;
	if (!GoRect(11, 13))
		return 0;
	if (!GoRect(11, 10))
		return 0;
	ASM_NPCInteract(11, 8);
	Sleep(SLEEP_TIME_DELIVE);
	ASM_ReadyOK(4);
	Sleep(SLEEP_TIME_DELIVE*2);
	if (!GoRect(7, 13))
		return 0;
	if (!GoRect(9, 20))
		return 0;
	if (!GoRect(9, 22))
		return 0;
	ASM_RoleWalk(4, 4);
	Sleep(SLEEP_TIME_DELIVE);
	dwWhileNum = 0;
	while (1)
	{
		//  �жϵ�ǰ����
		if (ASM_GetRoleCoorX() == 73 && ASM_GetRoleCoorY() == 60)
			break;
		dwWhileNum++;
		if (dwWhileNum > 20)
			return 0;
		Sleep(SLEEP_TIME_USUAL);
	}
	if (!GoRect(66, 70))
		return 0;
	if (!GoRect(63, 79))
		return 0;
	return 1;
}

void __cdecl OutputDebugStringFA(const char* format, ...)
{
	va_list v1Args;
	char* strBuffer = (char*)GlobalAlloc(GPTR, 4009);

	va_start(v1Args, format);
	_vsnprintf(strBuffer, 4096 - 1, format, v1Args);
	va_end(v1Args);
	strcat(strBuffer, "\n");
	OutputDebugStringA(strBuffer);
	GlobalFree(strBuffer);
	return;
}

void __cdecl OutputDebugStringFW(const wchar_t* format, ...)
{
	va_list v1Args;
	wchar_t* strBuffer = (wchar_t*)GlobalAlloc(GPTR, 4009);

	va_start(v1Args, format);
	_vswprintf_c(strBuffer, 4096 - 1, format, v1Args);
	va_end(v1Args);
	wcscat(strBuffer, L"\n");
	OutputDebugStringW(strBuffer);
	GlobalFree(strBuffer);
	return;
}
