#ifndef _PLAYBACKMEDIAHANDLE_H
#define _PLAYBACKMEDIAHANDLE_H

#include "SipHandleBase.h"
#include "SipMgr.h"

#include "PlaybackDef.h"
#include "StreamMgr.h"



#define DOCK_TYPE "NVR"
#define VIA "0"

class PlayBack : public SipHandleBase 
{
public:
	PlayBack(CSipMgr* pSipServer);
	~PlayBack(void);

public:
    //sip��ʷ�ļ�����
	static void SipPlaybackList(SipMessage& msg, int& iresult, void* context);
	//sip��ʷ�ļ��ط�
	static void SipPlaybackInvite(SipMessage& msg, int& iresult, void* context);
	//sip�طſ���-play
	static void SipPlaybackCtrlInfo_Play(SipMessage& msg, int& iresult, void* context);
	//sip�طſ���-pause
	static void SipPlaybackCtrlInfo_Pause(SipMessage& msg, int& iresult, void* context);
	//sip�طſ���-teardown
	static void SipPlaybackCtrlInfo_Teardown(SipMessage& msg, int& iresult, void* context);

	//�طŽ�����������message_status
	static void SipPlayback_MediaStatus(SipMessage& msg, int& iresult, void* context);

	static void ParseSubject(std::string& str, std::string& deviceAddr, int& index);

	static void FileListRequest(int index);
	static int SendFileList(char* msgBody, int len, char* fromIp, int fromPort, char* fromUsr);

public:
	static StreamMgr* m_pStreamMgr;

};

#endif

