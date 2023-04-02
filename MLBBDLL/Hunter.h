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


#define SLEEP_TIME_WALK 270				// 走路等待时间
#define SLEEP_TIME_USUAL 700			// 普通等待时间
#define SLEEP_TIME_WORK 4100			// 开始使用挖矿后结束的等待时间
#define SLEEP_TIME_DELIVE 1600			// 切图/传送点传送时间
#define SLEEP_TIME_PLAYSTRANG 2000		// 打怪过动画延时
#define SLEEP_TIME_INSPECT 4000			// 检测游戏状态线程检测时间

#define STANCE_NUMONE 1					// 挂机第一阶段(采集_贩卖物品)
#define STANCE_NUMTWO 2					// 挂机第二阶段(采集_医院回血)
#define STANCE_NUMTHREE 3				// 挂机第三阶段(采集_菜地采集)

//#define StrZero 0x2f302020				// 字符串"000/"


#define OPEN_FUNC_COLLTASK	 1		// 开启挂机采集功能[回城贩卖物品,医院治疗,前往挂机采集物品]
#define OPEN_FUNC_INSPECT	 2		// 开启游戏数据检测功能[检测是否遇怪自动打怪, 检测当前健康值使用恢复技能]
#define OPEB_FUNC_NEWPRESON  3		// 开始新手任务过图
#define OPEN_FUNC_AUTOPALYMOST 4	// 根据等级挂机打怪
#define OPEN_FUNC_TRADD		5		// 仓库号挂机接收物品/金币
#define OPEN_FUNC_TRANSFER		6	// 仓库号挂机接收物品/金币
#define OPEN_FUNC_AUTOPALYMOSTA 0xff	// 开始1-5级定点打怪
#define OPEN_FUNC_AUTOPALYMOSTB 0xfE	// 开始5-10级定点打怪
#define OPEN_FUNC_AUTOPALYMOSTC 0xfD	// 开始猎人村定点打怪

extern char ASM_GetAddreByte( long Addre);					// 获取指定地址里1个字节的数据
extern short ASM_GetAddreWord(long Addre);					// 获取指定地址里2个字节的数据
extern long ASM_GetAddreDword(long Addre);					// 获取指定地址里4个字节的数据
extern int ASM_GetRoleCoorX(void);							// 获取当前人物的X坐标
extern int ASM_GetRoleCoorY(void);							// 获取当前人物的Y坐标
extern void ASM_playerAttack(__int32 Index);				// 人物攻击(怪物格子)
extern void ASM_petAttack(__int32 Index, __int32 Num);		// 宠物攻击(怪物格子, 技能下标)
extern void ASM_RoleAddAttr(__int32 Index);					// 人物添加属性点([体力0,力量1,强度2, 速度3,魔法4])
extern void ASM_AddPetsAttr(__int32 Index);					// 宠物添加属性点([体力0,力量1,强度2, 速度3,魔法4])
extern void ASM_ComeBady(__int32 Index);					// 宠物出战([恢复0,待命1,出战2, 展示3])


extern void ASM_CallHome(void);								// 回城
extern void ASM_RoleWalk(unsigned short Direct, unsigned int GoStep);	// 走路(方向, 步长)

extern int ASM_NPCInteract(__int32 X, __int32 Y);		   // 选中NPC点击/传送点传送点击(X坐标,Y坐标)
//extern int ASM_RBtnDown(__int32 Direc);					// 右键点击
extern void ASM_DoctorReco( __int32 Index);				//使用护士/医生回血对话框(NPC_ID, 选项下标1开始)
extern void ASM_OpenShop(__int32 Index);					// 打开商店NPC贩卖对话框参数(打开商店第几栏 买/卖， 从1开始）
extern void ASM_ReadyOK(__int32 Index);					// NPC对话框操作(Index) 确定4/取消8/关闭1/下一步0x20
//extern void ASM_Print(char*);							// 游戏内输出打印
extern void ASM_SellShop(char* Index);						// 贩卖背包物品(贩卖物品(物品在背包中下标的字符串))
extern void ASM_DeleteGoods(__int32 Index);					// 丢弃背包物品(背包物品在背包中的下标,从1开始)
extern void ASM_WearItem(__int32 knapIndex, __int32 BodyIndex);	// 解除/装备背包物品(背包物品在背包中的下标,从1开始)

extern void ASM_UseSkill(__int32 Index, __int32 Num);		 // 工作技能使用技能(技能在技能栏下标, 技能等级下标)
extern void ASM_WorkOver(__int32 Index, __int32 Num);		// 快速结束技能(技能在技能栏下标, 技能等级下标)

//extern void ASM_useSkillRecov(__int32 Index, __int32 Num);	// 恢复健康技能使用(技能在技能栏下标, 技能等级下标), 从1开始
extern void ASM_useSkillRecov( __int32 Num);	// 恢复健康技能使用(技能在技能栏下标, 技能等级下标), 从1开始
extern void ASM_useRecovItemOne(__int32 Index);				// 使用恢复健康功能选择对象(自己或者队友),下标从1开始
extern void ASM_useRecovItemTwo(__int32 Index);				//使用恢复健康功能选择目标(自己或者宠物),下标从1开始
//extern void ASM_CloseRecov(void);							//使用恢复健康功能后关闭对话框

extern void ASM_IsAddArmy(void);							// 设置允许/禁止组队
extern void ASM_IsTrade(void);								// 设置允许/禁止交易
extern void ASM_AddArmy(void);								// 加入/退出队伍

extern void ASM_Trade(void);								// 发送交易请求
extern void ASM_TradeChoice(__int32 Index);					// 发送交易请求(选择交易对象,下标从0开始)
extern void ASM_TradeGoods(int, char* );					// 金币交易 // 交易物品(金币数量, 要交易的所有物品存放格子的字符串)
extern void ASM_TradeOK(void);								// 交易确认


// 队伍数组成员结构
struct _ROLERANKS
{
	int* RoleInfor;											// 指向个人信息,有值就是有人		
	int Unknown_One[11];
}ROLERANKS, *PROLERANKS;

struct _SKILL
{
	char Name[0x10];

}SKILL, *PSKILL;

#define  AppUser ((char*)0x0D306FC)			// 账户信息
#define  AppPass ((char*)0x0D304FC)			// 密码信息  0x0D2F4BC+0x1040
//#define  AppCurIP ((char*)0x0D29C08)		// 本地IP
#define  AppServerIP ((char*)0x0D2FFF8)		// 服务器IP

#define playerName ((char*)0xF674B0)		// 人物名字
#define playerLV ((int*)0xF673AC)			// 人物等级
#define playerMP ((char*)0xCD28F1)			// 人物魔法, 字符串类型
#define playerHP ((char*)0xCCD6B9)			// 人物血量, 字符串类型

#define playerX ((short int*)0xC01BAC)		// 人物当前X坐标(东)
#define playerY ((short int*)0xC01C74)		// 人物当前Y坐标(南)

#define playHelth ((char*)0xF6749C)			// 人物当前健康值 
#define playMoney ((int*)0xF674A4)			// 人物当前金币
#define playWorkExp ((int*)0xF673B4)		// 人物当前双倍工作经验剩余时间

#define playAttribut ((int*)0xCBC324)		// 人物当前拥有属性点
#define playPetsAttr ((int*)0xEF010C)		// 人物拥有的第一个宠物的属性

#define	RoleKnapBase ((short*)0xF6754C)	   // 人物背包起始基址 addre = (unsigned int)RoleKnapBase+ i* 0xc5c;
#define KnapBase ((int*)0x0CE2260)			// 物品背包起始基址	
#define MinTime ((int*)0xC80220)			// 物品采集时游戏保存的采集的时间

#define MostJMP ((int*)0X5A27E0)			// 怪物动画 0x05A17B0+1030
#define attackStatus ((int*)0x05A28F4)		// 打怪时谁的攻击回合 0x059C664+5260
#define battleArrBase ((int*)0x596644)		// 战斗队列数组起始基址
#define NPCID ((int*)0x0CBAA04)				// 存放当前选中NPC ID

#define SKILL_BASE ((char*)0xEA87A4)		// 技能数组的起始基址
#define SKILL_INDEX_BASE 0xEF001A			// 技能下标数组起始位置
#define SKILL_TREAT	0xF8C076AA				// 治疗技能名字字节码
//#define OffLine 0xf7759c					// 离线变成2

#define SKILL_STACE ((char*)0xF67538)		// 当前使用的技能序号(采集7/治疗13)

//#define CurSocialState ((int*)0xF6103E)		// 当前角色队伍信息
#define IsSocialState ((int*)0xF6753C)		// 当前社交状态
#define CurTradeState ((int*)0xC28364)		// 当前交易状态
#define CurTradeCfirm ((int*)0xC50DBC)		// 对方已选好物品
#define TradeRole	((char*)0xC80ABC)		// 交易对象的名字(0x0C7A74C+i*0x10)
#define CurRankBase ((int*)0xF7CF88)		// 当前队伍数组基址

typedef struct
{
	int X;
	int Y;
	//char* Name;
}NPCRoleClass;		//NPC/人物 信息结构


extern __declspec(dllexport) HANDLE OpenFunc(DWORD);							// 开启某项功能
extern __declspec(dllexport) void CloseFunc(HANDLE);						// 开启关闭项功能
//void GamePrint(char* str);					// 游戏窗口输出

DWORD GoRect(int x, int y);						// 走路斜走(X坐标, Y坐标) 
DWORD GoLect(int x, int y);						// 走路水平走(X坐标, Y坐标) 
void DeliPoint(int Index);						// 传送点定点传送
DWORD HealthRecov();							// 使用技能恢复健康,成功返回1失败返回0
void Battle(void);								// 自动打怪,怪物死完退出循环


int GetGoodsInex(int, int);						// 根据 物品类型/物品ID  获取'常用物品'在物品背包数组里的下标
int GetRoleGoodByName(char*);					// 根据物品名字查找物品在整个人物背包的位置,没有返回-1	
DWORD GetMoster(void);							// 获取有怪物格子索引
DWORD GetGroupNum();							// 获取当前队伍成员数量
int GetSkillIndexByName(char* Skill_NAME);		// 获取技能在技能栏的下标，错误返回-1
int GetSkillNumByName(char* Skill_NAME);		// 获取某个技能有多少数量等级的技能

void DiscardGoods(int[][2], int);				// 丢弃背包里所有物品(白名单起始位置,白名单大小)
DWORD SetSocial(DWORD);							// 开启/关闭 某项社交状态
void SetRoleAttr();								// 设置 人物/宠物属性

/* 任务流程*/
extern __declspec(dllexport) void HangUp(void);								// 自动挂机采集
extern __declspec(dllexport) void HunterInspect(void);						// 开启检测游戏数据,人物状态状态
extern __declspec(dllexport) DWORD ComeNewBady();							// 新号挂机(新手任务-->自动打怪)

void WareHouse(void);							// 仓库号挂机
DWORD GoWare(void);								// 前往仓库号的点
DWORD GoTrade(void);							// 功能号转移金币例程
DWORD GoldTransfer(void);						// 金币交易

DWORD GoField(void);							// 前往采集点
void Collec(void);								// 采集物品
DWORD GoShop(void);								// 前往贩卖点
void Peddling(void);							// 物品贩卖
DWORD GoHospi(void);							// 前往医院
void Reply(int Index);							// 医生护士回血(代表护士, 1代表医生)


//DWORD GoHunterOffer();
void CreateNewRole(void);						// 新角色过新手任务
DWORD GoNewPerson(void);						// 新手任务地牢出生
DWORD GoNewPersonOver(void);					// 新手在国王处对话
void AutoPlayMost(void);						// 根据等级自动定点打怪

DWORD TheWorkTime(void);						// 领取双倍经验
DWORD PlayNovic(void);							// 初级打怪流程(1-5级单人路线)
DWORD PlayPrimary(void);						// 初级打怪流程(5-10级单人路线)
DWORD PlayLower(void);							// 低级打怪流程(10-15级路线)
DWORD PlayTeBie(void);							// 低级打怪(猎人村起点)
DWORD GoGroup();								// 前往组队地点
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

