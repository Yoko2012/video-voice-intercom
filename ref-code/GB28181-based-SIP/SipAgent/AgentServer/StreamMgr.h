#pragma once

#include "lock.h"
#include <list>
/*
ʵʱ��Ƶ���Ĺ���
*/

//����Ϣ
struct StreamInfo
{
	int streamIndex;    //�����
	char addrCode[24];   //sip�豸��ַ����
	void* sessionHandle;  //sip�Ự���

	StreamInfo()
	{
		streamIndex = 0;
		memset(addrCode, 0, sizeof(addrCode));
		sessionHandle = NULL;
	}

	StreamInfo& operator=(const StreamInfo& info)
	{
		streamIndex = info.streamIndex;
		strcpy(addrCode, info.addrCode);
		sessionHandle = info.sessionHandle;

		return *this;
	}

};


class StreamMgr
{
public:
	StreamMgr(void);
	~StreamMgr(void);


public:
	void OpenStream(StreamInfo* stream);
	void CloseStream(void* handle, StreamInfo* stream); 

	int GetStreamInfo(void* handle, StreamInfo* stream);

public:
	std::list<StreamInfo> m_StreamList;

	static lock_summary::critical_lock ms_crtlLock;

};
