#pragma once
#include <stdio.h>
#include <Windows.h>

#define _CRT_SECURE_NO_WARNINGS
void __cdecl OutputDebugStringFA(const char* format, ...);
void __cdecl OutputDebugStringFW(const wchar_t* format, ...);

#ifdef _DEBUG
#define WinPrintA OutputDebugStringFA
#define WinPrintW OutputDebugStringFW
#else
#define WinPrintA
#define WinPrintW
#endif


#define SLEEP_TIME_WALK 270				// ��·�ȴ�ʱ��
#define SLEEP_TIME_USUAL 700			// ��ͨ�ȴ�ʱ��
#define SLEEP_TIME_WORK 4100			// ��ʼʹ���ڿ������ĵȴ�ʱ��
#define SLEEP_TIME_DELIVE 1600			// ��ͼ/���͵㴫��ʱ��
#define SLEEP_TIME_PLAYSTRANG 2000		// ��ֹ�������ʱ
#define SLEEP_TIME_INSPECT 4000			// �����Ϸ״̬�̼߳��ʱ��

#define STANCE_NUMONE 1					// �һ���һ�׶�(�ɼ�_������Ʒ)
#define STANCE_NUMTWO 2					// �һ��ڶ��׶�(�ɼ�_ҽԺ��Ѫ)
#define STANCE_NUMTHREE 3				// �һ������׶�(�ɼ�_�˵زɼ�)

//#define StrZero 0x2f302020				// �ַ���"000/"


#define OPEN_FUNC_COLLTASK	 1		// �����һ��ɼ�����[�سǷ�����Ʒ,ҽԺ����,ǰ���һ��ɼ���Ʒ]
#define OPEN_FUNC_INSPECT	 2		// ������Ϸ���ݼ�⹦��[����Ƿ������Զ����, ��⵱ǰ����ֵʹ�ûָ�����]
#define OPEB_FUNC_NEWPRESON  3		// ��ʼ���������ͼ
#define OPEN_FUNC_AUTOPALYMOST 4	// ���ݵȼ��һ����
#define OPEN_FUNC_TRADD		5		// �ֿ�Źһ�������Ʒ/���
#define OPEN_FUNC_TRANSFER		6	// �ֿ�Źһ�������Ʒ/���
#define OPEN_FUNC_AUTOPALYMOSTA 0xff	// ��ʼ1-5��������
#define OPEN_FUNC_AUTOPALYMOSTB 0xfE	// ��ʼ5-10��������
#define OPEN_FUNC_AUTOPALYMOSTC 0xfD	// ��ʼ���˴嶨����

extern char ASM_GetAddreByte( long Addre);					// ��ȡָ����ַ��1���ֽڵ�����
extern short ASM_GetAddreWord(long Addre);					// ��ȡָ����ַ��2���ֽڵ�����
extern long ASM_GetAddreDword(long Addre);					// ��ȡָ����ַ��4���ֽڵ�����
extern int ASM_GetRoleCoorX(void);							// ��ȡ��ǰ�����X����
extern int ASM_GetRoleCoorY(void);							// ��ȡ��ǰ�����Y����
extern void ASM_playerAttack(__int32 Index);				// ���﹥��(�������)
extern void ASM_petAttack(__int32 Index, __int32 Num);		// ���﹥��(�������, �����±�)
extern void ASM_RoleAddAttr(__int32 Index);					// ����������Ե�([����0,����1,ǿ��2, �ٶ�3,ħ��4])
extern void ASM_AddPetsAttr(__int32 Index);					// ����������Ե�([����0,����1,ǿ��2, �ٶ�3,ħ��4])
extern void ASM_ComeBady(__int32 Index);					// �����ս([�ָ�0,����1,��ս2, չʾ3])


extern void ASM_CallHome(void);								// �س�
extern void ASM_RoleWalk(unsigned short Direct, unsigned int GoStep);	// ��·(����, ����)

extern int ASM_NPCInteract(__int32 X, __int32 Y);		   // ѡ��NPC���/���͵㴫�͵��(X����,Y����)
//extern int ASM_RBtnDown(__int32 Direc);					// �Ҽ����
extern void ASM_DoctorReco( __int32 Index);				//ʹ�û�ʿ/ҽ����Ѫ�Ի���(NPC_ID, ѡ���±�1��ʼ)
extern void ASM_OpenShop(__int32 Index);					// ���̵�NPC�����Ի������(���̵�ڼ��� ��/���� ��1��ʼ��
extern void ASM_ReadyOK(__int32 Index);					// NPC�Ի������(Index) ȷ��4/ȡ��8/�ر�1/��һ��0x20
//extern void ASM_Print(char*);							// ��Ϸ�������ӡ
extern void ASM_SellShop(char* Index);						// ����������Ʒ(������Ʒ(��Ʒ�ڱ������±���ַ���))
extern void ASM_DeleteGoods(__int32 Index);					// ����������Ʒ(������Ʒ�ڱ����е��±�,��1��ʼ)
extern void ASM_WearItem(__int32 knapIndex, __int32 BodyIndex);	// ���/װ��������Ʒ(������Ʒ�ڱ����е��±�,��1��ʼ)

extern void ASM_UseSkill(__int32 Index, __int32 Num);		 // ��������ʹ�ü���(�����ڼ������±�, ���ܵȼ��±�)
extern void ASM_WorkOver(__int32 Index, __int32 Num);		// ���ٽ�������(�����ڼ������±�, ���ܵȼ��±�)

//extern void ASM_useSkillRecov(__int32 Index, __int32 Num);	// �ָ���������ʹ��(�����ڼ������±�, ���ܵȼ��±�), ��1��ʼ
extern void ASM_useSkillRecov( __int32 Num);	// �ָ���������ʹ��(�����ڼ������±�, ���ܵȼ��±�), ��1��ʼ
extern void ASM_useRecovItemOne(__int32 Index);				// ʹ�ûָ���������ѡ�����(�Լ����߶���),�±��1��ʼ
extern void ASM_useRecovItemTwo(__int32 Index);				//ʹ�ûָ���������ѡ��Ŀ��(�Լ����߳���),�±��1��ʼ
//extern void ASM_CloseRecov(void);							//ʹ�ûָ��������ܺ�رնԻ���

extern void ASM_IsAddArmy(void);							// ��������/��ֹ���
extern void ASM_IsTrade(void);								// ��������/��ֹ����
extern void ASM_AddArmy(void);								// ����/�˳�����

extern void ASM_Trade(void);								// ���ͽ�������
extern void ASM_TradeChoice(__int32 Index);					// ���ͽ�������(ѡ���׶���,�±��0��ʼ)
extern void ASM_TradeGoods(int, char* );					// ��ҽ��� // ������Ʒ(�������, Ҫ���׵�������Ʒ��Ÿ��ӵ��ַ���)
extern void ASM_TradeOK(void);								// ����ȷ��


// ���������Ա�ṹ
struct _ROLERANKS
{
	int* RoleInfor;											// ָ�������Ϣ,��ֵ��������		
	int Unknown_One[11];
}ROLERANKS, *PROLERANKS;

struct _SKILL
{
	char Name[0x10];

}SKILL, *PSKILL;

#define  AppUser ((char*)0x0D306FC)			// �˻���Ϣ
#define  AppPass ((char*)0x0D304FC)			// ������Ϣ  0x0D2F4BC+0x1040
//#define  AppCurIP ((char*)0x0D29C08)		// ����IP
#define  AppServerIP ((char*)0x0D2FFF8)		// ������IP

#define playerName ((char*)0xF674B0)		// ��������
#define playerLV ((int*)0xF673AC)			// ����ȼ�
#define playerMP ((char*)0xCD28F1)			// ����ħ��, �ַ�������
#define playerHP ((char*)0xCCD6B9)			// ����Ѫ��, �ַ�������

#define playerX ((short int*)0xC01BAC)		// ���ﵱǰX����(��)
#define playerY ((short int*)0xC01C74)		// ���ﵱǰY����(��)

#define playHelth ((char*)0xF6749C)			// ���ﵱǰ����ֵ 
#define playMoney ((int*)0xF674A4)			// ���ﵱǰ���
#define playWorkExp ((int*)0xF673B4)		// ���ﵱǰ˫����������ʣ��ʱ��

#define playAttribut ((int*)0xCBC324)		// ���ﵱǰӵ�����Ե�
#define playPetsAttr ((int*)0xEF010C)		// ����ӵ�еĵ�һ�����������

#define	RoleKnapBase ((short*)0xF6754C)	   // ���ﱳ����ʼ��ַ addre = (unsigned int)RoleKnapBase+ i* 0xc5c;
#define KnapBase ((int*)0x0CE2260)			// ��Ʒ������ʼ��ַ	
#define MinTime ((int*)0xC80220)			// ��Ʒ�ɼ�ʱ��Ϸ����Ĳɼ���ʱ��

#define MostJMP ((int*)0X5A27E0)			// ���ﶯ�� 0x05A17B0+1030
#define attackStatus ((int*)0x05A28F4)		// ���ʱ˭�Ĺ����غ� 0x059C664+5260
#define battleArrBase ((int*)0x596644)		// ս������������ʼ��ַ
#define NPCID ((int*)0x0CBAA04)				// ��ŵ�ǰѡ��NPC ID

#define SKILL_BASE ((char*)0xEA87A4)		// �����������ʼ��ַ
#define SKILL_INDEX_BASE 0xEF001A			// �����±�������ʼλ��
#define SKILL_TREAT	0xF8C076AA				// ���Ƽ��������ֽ���
//#define OffLine 0xf7759c					// ���߱��2

#define SKILL_STACE ((char*)0xF67538)		// ��ǰʹ�õļ������(�ɼ�7/����13)

//#define CurSocialState ((int*)0xF6103E)		// ��ǰ��ɫ������Ϣ
#define IsSocialState ((int*)0xF6753C)		// ��ǰ�罻״̬
#define CurTradeState ((int*)0xC28364)		// ��ǰ����״̬
#define CurTradeCfirm ((int*)0xC50DBC)		// �Է���ѡ����Ʒ
#define TradeRole	((char*)0xC80ABC)		// ���׶��������(0x0C7A74C+i*0x10)
#define CurRankBase ((int*)0xF7CF88)		// ��ǰ���������ַ

typedef struct
{
	int X;
	int Y;
	//char* Name;
}NPCRoleClass;		//NPC/���� ��Ϣ�ṹ


extern __declspec(dllexport) HANDLE OpenFunc(DWORD);							// ����ĳ���
extern __declspec(dllexport) void CloseFunc(HANDLE);						// �����ر����
//void GamePrint(char* str);					// ��Ϸ�������

DWORD GoRect(int x, int y);						// ��·б��(X����, Y����) 
DWORD GoLect(int x, int y);						// ��·ˮƽ��(X����, Y����) 
void DeliPoint(int Index);						// ���͵㶨�㴫��
DWORD HealthRecov();							// ʹ�ü��ָܻ�����,�ɹ�����1ʧ�ܷ���0
void Battle(void);								// �Զ����,���������˳�ѭ��


int GetGoodsInex(int, int);						// ���� ��Ʒ����/��ƷID  ��ȡ'������Ʒ'����Ʒ������������±�
int GetRoleGoodByName(char*);					// ������Ʒ���ֲ�����Ʒ���������ﱳ����λ��,û�з���-1	
DWORD GetMoster(void);							// ��ȡ�й����������
DWORD GetGroupNum();							// ��ȡ��ǰ�����Ա����
int GetSkillIndexByName(char* Skill_NAME);		// ��ȡ�����ڼ��������±꣬���󷵻�-1
int GetSkillNumByName(char* Skill_NAME);		// ��ȡĳ�������ж��������ȼ��ļ���

void DiscardGoods(int[][2], int);				// ����������������Ʒ(��������ʼλ��,��������С)
DWORD SetSocial(DWORD);							// ����/�ر� ĳ���罻״̬
void SetRoleAttr();								// ���� ����/��������

/* ��������*/
extern __declspec(dllexport) void HangUp(void);								// �Զ��һ��ɼ�
extern __declspec(dllexport) void HunterInspect(void);						// ���������Ϸ����,����״̬״̬
extern __declspec(dllexport) DWORD ComeNewBady();							// �ºŹһ�(��������-->�Զ����)

void WareHouse(void);							// �ֿ�Źһ�
DWORD GoWare(void);								// ǰ���ֿ�ŵĵ�
DWORD GoTrade(void);							// ���ܺ�ת�ƽ������
DWORD GoldTransfer(void);						// ��ҽ���

DWORD GoField(void);							// ǰ���ɼ���
void Collec(void);								// �ɼ���Ʒ
DWORD GoShop(void);								// ǰ��������
void Peddling(void);							// ��Ʒ����
DWORD GoHospi(void);							// ǰ��ҽԺ
void Reply(int Index);							// ҽ����ʿ��Ѫ(����ʿ, 1����ҽ��)


//DWORD GoHunterOffer();
void CreateNewRole(void);						// �½�ɫ����������
DWORD GoNewPerson(void);						// ����������γ���
DWORD GoNewPersonOver(void);					// �����ڹ������Ի�
void AutoPlayMost(void);						// ���ݵȼ��Զ�������

DWORD TheWorkTime(void);						// ��ȡ˫������
DWORD PlayNovic(void);							// �����������(1-5������·��)
DWORD PlayPrimary(void);						// �����������(5-10������·��)
DWORD PlayLower(void);							// �ͼ��������(10-15��·��)
DWORD PlayTeBie(void);							// �ͼ����(���˴����)
DWORD GoGroup();								// ǰ����ӵص�
//DWORD GroupBoss();							




struct seat {
	char v1[50];
	int v2;
};

//class base
//{
//	struct seat;
//	//int (*func)(int, int, int);
//};

//{
//	//base* xxx = (base*)123;
//	//xxx->func(1, 2, 3);
//	int v1 = 0;
//	int v2 = 0;
//	v1 = class1->func1(eax, edx, 0, ecx);
//	v2 = class2->func(v1);
//	func( (int)class1, v2);
//
//}

