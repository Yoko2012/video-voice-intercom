#pragma once

//����sip����keepalive����������sip�������󵽵�����ϵͳ

#include "SipMgr.h"
#include "SipHandleBase.h"

class TaskTimer;

class SipHeartBeat : SipHandleBase
{
public:
	SipHeartBeat(CSipMgr* pServer);
	~SipHeartBeat(void);

public:
	//��������
	void HeartBeatServiceStart();
	//������������
	static void SipHeartBeatServiceMgr(SipMessage& msg, int& iresult, void* context);
	static void CheckAllUacHeartBeat(void* lpVoid);

	//����������������
	static void HeartBeatProc(void* param);
public:
	//������鶨ʱ��
	TaskTimer* m_pCheckUacStateTimer;
	//��������������ʱ��
	TaskTimer* m_pHearBeatTimer;

	//
	static int m_HeartBeatFailedNum;
	//
	static int m_SNIndex;
};
