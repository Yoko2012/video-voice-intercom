#pragma once

//��������ע�����󡢹����¼��򱾼������ע������
#include "SipMgr.h"
#include "SipHandleBase.h"


class TaskTimer;

class SipRegister : SipHandleBase
{
public:
	SipRegister(CSipMgr* pServer);
	~SipRegister(void);

public:
	//����ע�����
	static void RegisterServiceMgr(SipMessage& msg, int& iresult, void* context);
	static void RegisterServiceSuccess(SipMessage& msg, int& iresult, void* context);

	int RegisterServiceStart();
	static void RegisterProc(void* param);

	//�����ṩע��
	int Register(int expires);

public:
	TaskTimer* m_RegTimer;

	char m_SysCheckDateTm[64];

	//�Ƿ��Ѿ�ע��
	static bool g_bRegisted;
	//ע���handle
	static RvSipRegClientHandle g_hRegClient;
};
