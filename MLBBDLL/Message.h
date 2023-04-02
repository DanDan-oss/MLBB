#pragma once
#include <Windows.h>

// SendMessage(hwnd, WM_CONTROL_FUNC, VK_FUNC_NUM, TRUE);		// 开启任务流程(, )
#define WM_CONTROL_FUNC  WM_USER+1			// 任务流程控制
#define VK_OPEN_COLLTASK	1			// 开始挂机任务
#define VK_OPEN_NEWPRESON	2			// 新手任务

#define VK_OPEN_TRADE		5			// 交易挂机
#define GOLD_TRANS	1					// 仓库号挂机
#define GOLD_TRANSFER	2				// 交易物品给仓库号	

#define VK_OPEN_AUTOPLAY    3			// 定点挂机打怪
#define GD_OPEN_PLAYMOST	1			// 根据等级自动选择打怪地点
#define GD_OPEN_ONE	0xff				// 定点打怪<=5级
#define GD_OPEN_TWO	0xfe				// 定点打怪5<=10级
#define GO_OPEN_CCC 0x1d				// 定点打怪,开始猎人村定点打怪村庄开始


#define WM_CONTROL_INSPECT WM_USER+2		// 监控数据
#define VK_AUTOMONSTER	1					// 自动打怪检测

#define WM_SETWINDOW_BOTTON WM_USER+100		// 窗口鼠标屏蔽


#define WM_GROUP  WM_USER+3					// 打怪时是否开启组队
#define VK_GROUPBOSS	1					// 队长组队
#define VK_GROUPMEMBER	2					// 队员组队

