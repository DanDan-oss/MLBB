#pragma once
#include <Windows.h>

// SendMessage(hwnd, WM_CONTROL_FUNC, VK_FUNC_NUM, TRUE);		// ������������(, )
#define WM_CONTROL_FUNC  WM_USER+1			// �������̿���
#define VK_OPEN_COLLTASK	1			// ��ʼ�һ�����
#define VK_OPEN_NEWPRESON	2			// ��������

#define VK_OPEN_TRADE		5			// ���׹һ�
#define GOLD_TRANS	1					// �ֿ�Źһ�
#define GOLD_TRANSFER	2				// ������Ʒ���ֿ��	

#define VK_OPEN_AUTOPLAY    3			// ����һ����
#define GD_OPEN_PLAYMOST	1			// ���ݵȼ��Զ�ѡ���ֵص�
#define GD_OPEN_ONE	0xff				// ������<=5��
#define GD_OPEN_TWO	0xfe				// ������5<=10��
#define GO_OPEN_CCC 0x1d				// ������,��ʼ���˴嶨���ִ�ׯ��ʼ


#define WM_CONTROL_INSPECT WM_USER+2		// �������
#define VK_AUTOMONSTER	1					// �Զ���ּ��

#define WM_SETWINDOW_BOTTON WM_USER+100		// �����������


#define WM_GROUP  WM_USER+3					// ���ʱ�Ƿ������
#define VK_GROUPBOSS	1					// �ӳ����
#define VK_GROUPMEMBER	2					// ��Ա���

