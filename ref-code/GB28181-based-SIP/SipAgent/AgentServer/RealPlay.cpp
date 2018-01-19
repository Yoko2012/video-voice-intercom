#include "StdAfx.h"
#include "RealPlay.h"

#include "GBStandard.h"
#include "GlobalCtrl.h"
#include "config/ProxyConfig.h"
#include "BcAdaptor.h"

#include <WinSock2.h>




std::list<RealPlay::RealMedia> RealPlay::ms_RealStreamMgr;
 lock_summary::critical_lock RealPlay::ms_crtlLock;


RealPlay::RealPlay(CSipMgr* sipServer)
{
	if (sipServer != NULL)
	{
		sipServer->Register(INVITE, PLAY, &(RealPlay::SipLiveStreamOpen), this);
		sipServer->Register(INVITE, "", &(RealPlay::SipLiveStreamOpen), this);
		sipServer->Register(BYE, "", &(RealPlay::SipLiveStreamClose), this);
	}
}

RealPlay::~RealPlay(void)
{
}


//INVITE �������Ӧ
void RealPlay::SipLiveStreamOpen(SipMessage& msg, int& iresult, void* context)
{
	int nRet = 0;
	int err = 0;

	const int maxSdpSize = 8*1024;
	const char* pvia = "0";

	std::string subject = msg.GetSubject();
	//����subject�� deviceaddr�������
	std::string deviceAddr;
	int index;
	ParseSubject(subject, deviceAddr, index);

	//�õ�ý���������߶˿�
	std::string sReq = msg.m_pMsgBody;
	RvSdp rvsdp;
	if (rvsdp.ParseSdpInfo((char*)sReq.c_str(), sReq.length()) != 0)
	{
		_LOG(LOG_LEVEL_DEBUG, "����sdpʧ��");
	}

	//TODO:������invite���ı�sdp��Ϣ
	int reqDevId = 0;
	int reqDevType = 0;
	GBSysCode::getDevTypeAndId(deviceAddr.c_str(), &reqDevType, &reqDevId);

	if (reqDevType != Decoder_Code && reqDevType != VGA_Code)
	{
		Origin o;
		cconnect c;
		mediaDescr m;
		rvsdp.getO(&o);
		rvsdp.getC(&c);
		rvsdp.getM(&m);
		int sdpPort = m.mediades[0].mport;
		std::string sdpIP = o.addr;
		if (sdpIP.length() == 0)
		{
			sdpIP = c.addr;
		}
		if (sdpIP.length() == 0 || sdpPort == 0)
		{
			_LOG(LOG_LEVEL_DEBUG,"SipLiveStreamOpen sdp��ȡip portʧ��\n");
			return ;
		}

		//�õ�ý���������߶˿�
		unsigned short bindPort = 23568;
		unsigned int bindIp = inet_addr(GLOBJ(gConfig)->getSiplocalIp());
		RealPlay::RealMedia media;
		media.devId = reqDevId;
		media.devType = reqDevType;
		//test
		//sdpIP = "35.34.9.201";
		//sdpPort = 8806;
#if 1
		media.hReal = GLOBJ(gAdaptor)->realTranslate( reqDevId, pvia, sdpIP.c_str(), (unsigned short)sdpPort, &err, &bindIp, &bindPort );
    if (media.hReal == NULL)
		{
			_LOG(LOG_LEVEL_ERROR, "adaptor_realTranslate failed");
			iresult = SIP_BADREQUEST;
			return ;
		}

		RealStreamAdd(&media);

#else
		//sendhik((char*)sdpIP.c_str(), sdpPort,0 );
		//bindPort = 5500;
#endif

		rvsdp.setO_user((char*)deviceAddr.c_str());
		//rvsdp.setO_user(gConfig->getSiplocalUser());
		rvsdp.SetNetAddressAndPort(GLOBJ(gConfig)->getSiplocalIp(), bindPort);
		//��ȡ�µ�sdp��Ϣ
		char cSdp[maxSdpSize] = {0};
		rvsdp.GetSdpMsgBuf(cSdp, maxSdpSize);
		msg.setSipMessageBody(cSdp, strlen(cSdp));


		msg.setContact(msg.from_user(), GLOBJ(gConfig)->getSiplocalIp(), GLOBJ(gConfig)->getSiplocalPort());

	}

	iresult = SIP_SUCCESS;

/*

	//�ظ�ack
	msg.setCallLegHandle((void*)msg.m_hCallLeg);
	nRet = CSipMgr::doSipRequest(ACK, msg);
	if (nRet != SIP_OK)
	{
		_LOG(LOG_LEVEL_DEBUG, "INVITE �ظ�ACKʧ��");
	}*/

}


void RealPlay::SipLiveStreamClose(SipMessage& msg, int& iresult, void* context)
{
	

	int devId;
	int devType;
	GBSysCode::getDevTypeAndId(msg.to_user(), &devType, &devId);
	HANDLE hReal = RealStreamGet(devId, devType);

	//close
	GLOBJ(gAdaptor)->realClose(hReal);

	RealStreamDel(devId, devType);

	iresult = SIP_SUCCESS;
}


void RealPlay::RealStreamAdd(RealMedia* stream)
{
	if (stream == NULL)
	{
		return ;
	}

	lock_summary::scope_lock lock(&ms_crtlLock);
	ms_RealStreamMgr.push_back(*stream);
}


void RealPlay::RealStreamDel(int devId, int devType)
{
	lock_summary::scope_lock lock(&ms_crtlLock);
	if (ms_RealStreamMgr.size() <= 0)
	{
		return ;
	}

	std::list<RealMedia>::iterator it;
	for (it = ms_RealStreamMgr.begin(); it != ms_RealStreamMgr.end(); it++)
	{
		if (it->devId == devId && it->devType == devType)
		{
			ms_RealStreamMgr.erase(it);
			break;
		}
	}
}


HANDLE RealPlay::RealStreamGet(int devId, int devType)
{
	lock_summary::scope_lock lock(&ms_crtlLock);
	std::list<RealMedia>::iterator it;
	for (it = ms_RealStreamMgr.begin(); it != ms_RealStreamMgr.end(); it++)
	{
		if (it->devId == devId && it->devType == devType)
		{
			return it->hReal;
		}
	}

	return NULL;
}


void RealPlay::ParseSubject(std::string& str, std::string& deviceAddr, int& index)
{
	std::string buf = str;
	size_t pos = buf.find_first_of(':', 0);
	//����ͷ��Address ID
	std::string addr = buf.substr(0, pos);
	size_t pos2 = buf.find_first_of(':', pos + 1);
	std::string indexStr = buf.substr(pos2 + 1);

	deviceAddr = addr;
	index = atoi(indexStr.c_str());
}



typedef struct
{
	unsigned short cc:4;
	unsigned short extbit:1;
	unsigned short padbit:1;
	unsigned short version:2;
	unsigned short paytype:7;
	unsigned short markbit:1;

	unsigned short seq_number; /**< ���ͱ��,�ۼ� */
	unsigned int timestamp;    /**< ʱ��� */
	unsigned int ssrc;
	// char data[0];
}RtpHeader;


void RealPlay::sendhik(char* ip, int port, int bindPort)
{
	// TODO: Add your control notification handler code here

	int ret = 0;
	SOCKET hSocket  = INVALID_SOCKET;

	hSocket = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in dstAddr;
	dstAddr.sin_family = AF_INET;
	dstAddr.sin_port   = htons( port );
	dstAddr.sin_addr.S_un.S_addr = inet_addr( ip );


	//udp��һ������ķ��Ͷ˿�
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_port = htons(5500);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(GLOBJ(gConfig)->getSiplocalIp());
	ret = bind( hSocket,(struct sockaddr *)&addr, sizeof(struct sockaddr_in));

	if( ret != 0 )
	{
		return ;
	}


	FILE *file = fopen( "F:/111.pcapng", "rb" );

	FILE *dst = fopen( "f:/hk.ps", "wb" );

	fseek( file, 0, SEEK_END );

	int total = ftell( file );

	fseek( file, 0, SEEK_SET );

	int offset = 0;

	unsigned short len = 0;

	char *pData = (char*)malloc( total );

	char *pRtp = NULL;
	int rtpLen = 0;

	RtpHeader *pHdr = NULL;

	fread( pData, 1, total, file );

	unsigned int *pVal = NULL;
	char *temp = NULL;

	for( int i = 0; i < total; i++ )
	{
		temp = pData + i;
		pVal = (unsigned int*)temp;
		if( *pVal == 0x83af2478 )
		{
			temp += 38;

			len = *((unsigned short*)(temp));

			len = htons( len );

			pRtp = temp + 4;

			rtpLen = len - 8;

			temp += 16;

			//fwrite( temp, 1, len - 20, dst );

			ret = sendto( hSocket, pRtp, rtpLen, 0, (const sockaddr*)&dstAddr, sizeof(dstAddr) );

			pHdr = (RtpHeader*)pRtp;
			if( pHdr->markbit == 1 )
			{
				Sleep( 40 );
			}
		}

		//Sleep( 1 );
	}

	fclose( dst );
	fclose( file );
	free( pData );
	if( hSocket != INVALID_SOCKET )
	{
		closesocket( hSocket );
	}

}

