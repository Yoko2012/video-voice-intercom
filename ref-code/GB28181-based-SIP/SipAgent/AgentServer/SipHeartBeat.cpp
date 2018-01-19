#include "SipHeartBeat.h"
#include "log4z.h"

#include "../Utility/TaskTimer.h"
#include "../GlobalCtrl.h"
#include "../config/ProxyConfig.h"

#include "SipRegister.h"

#define TIME_OUT 35

#define _CLIENT


int SipHeartBeat::m_HeartBeatFailedNum = 0;
int SipHeartBeat::m_SNIndex = 1621;

SipHeartBeat::SipHeartBeat(CSipMgr* pServer)
{
	if (pServer != NULL)
	{
		pServer->Register(MESSAGE, HEARTBEAT, &(SipHeartBeat::SipHeartBeatServiceMgr), this);
	}

	//���uac�Ƿ�����״̬��ʱ��
	//m_pCheckUacStateTimer = new TaskTimer(GLOBJ(gConfig)->getSip_heartBeatTm());
	//��������������ʱ��
	m_pHearBeatTimer = new TaskTimer(GLOBJ(gConfig)->getSip_heartBeatTm() - 10);

}


SipHeartBeat::~SipHeartBeat(void)
{
	//delete m_pCheckUacStateTimer;
	//m_pCheckUacStateTimer = NULL;

	delete m_pHearBeatTimer;
	m_pHearBeatTimer = NULL;
}

void SipHeartBeat::HeartBeatServiceStart()
{
#ifndef _CLIENT
	if (m_pCheckUacStateTimer != NULL)
	{
		m_pCheckUacStateTimer->setTimerFun(CheckAllUacHeartBeat, this);
		m_pCheckUacStateTimer->start();
	}
#else

	if (m_pHearBeatTimer != NULL)
	{
		m_pHearBeatTimer->setTimerFun(HeartBeatProc, this);
		m_pHearBeatTimer->start();
	}
#endif

}

/*
1 ����������ȡ����Ϣ
2 ȥuaclist����ʱ�������״̬
*/
void SipHeartBeat::SipHeartBeatServiceMgr(SipMessage& msg, int& iresult, void* context)
{
#if 0
	int curTm = GetTickCount();
	std::string addr = msg.from_ip();
	int port = msg.from_port();
	SipUACManager::uacInfo info;
	if (gUacMgr.GetUacInfo((char*)addr.c_str(), port, &info))
	{
		//�ж�
		if (curTm - info.heartTm <= gConfig->getSip_heartBeatTm()*1000 || info.heartTm == 0)
		{
			//����
			_LOG(LOG_LEVEL_DEBUG, "�豸��%s:%d��������", addr.c_str(), port);
		}
		else
		{
			_LOG(LOG_LEVEL_DEBUG, "�豸��%s:%d�����쳣", addr.c_str(), port);
		}
		gUacMgr.SetUacOnlineState((char*)addr.c_str(), port, curTm, true);

		msg.setSipMessageBody("", 0);
		iresult = SIP_SUCCESS;
	}
	else
	{
		iresult = SIP_NOTIMPLEMENT;
	}

#endif
}


void SipHeartBeat::CheckAllUacHeartBeat(void* lpVoid)
{
	//int curTm = GetTickCount();
	//gUacMgr.CheckAllUacHeartBeat(curTm);

}


void SipHeartBeat::HeartBeatProc(void* param)
{
	while (!SipRegister::g_bRegisted)
	{
		Sleep(500);
	}

	SipMessage msg;
	msg.setFrom(GLOBJ(gConfig)->getSiplocalUser(), GLOBJ(gConfig)->getSiplocalIp(), GLOBJ(gConfig)->getSiplocalPort());
	msg.setTo(GLOBJ(gConfig)->getSipremoteUser(), GLOBJ(gConfig)->getSipremoteIp(), GLOBJ(gConfig)->getSipremotePort());
	msg.setUri(GLOBJ(gConfig)->getSipremoteIp(), GLOBJ(gConfig)->getSipremotePort());
	std::string cty = "Application/MANSCDP+xml";
	msg.setContentType((char*)cty.c_str(), cty.length());
	//��ʱ�̶�
	char strIndex[16] = {0};
	sprintf(strIndex, "%d", m_SNIndex++);
	std::string keepalive = "<?xml version=\"1.0\"?>\r\n";
	keepalive += "<Notify>\r\n";
	keepalive += "<CmdType>Keepalive</CmdType>\r\n";
	keepalive += "<SN>";
	keepalive += strIndex;
	keepalive += "</SN>\r\n";
	keepalive += "<DeviceID>";
	keepalive += GLOBJ(gConfig)->getSiplocalUser();
	keepalive += "</DeviceID>\r\n"; 
	keepalive += "<Status>OK</Status>\r\n";
  keepalive += "</Notify>\r\n";
	
	msg.setSipMessageBody(keepalive.c_str(), strlen(keepalive.c_str()));
	int ret = 0;
	ret = CSipMgr::doSipRequest(MESSAGE, msg);
	if (ret == SIP_OK)
	{
		//if (msg.status_code() == SIP_SUCCESS)
		{
			_LOG(LOG_LEVEL_DEBUG, "SIP������������!");
		}
		//else
		//{
		//	_LOG(LOG_LEVEL_DEBUG, "SIP��������%d������ע��,msg: %s", msg.status_code(), msg.sipMessageBody());
			//Register();
		//}
	}
	else
	{
		if (ret == SIP_REQUEST_TIMEOUT)
		{
			//��ʱ�Ѵﵽ90�룬����ע��
			_LOG(LOG_LEVEL_DEBUG, "SIP�������ͳ�ʱ");
			//service->proxyRegister();
		}
		else
		{
			_LOG(LOG_LEVEL_DEBUG, "SIP��������ʧ��");
			//����ʧ������
			m_HeartBeatFailedNum++;
			if (m_HeartBeatFailedNum >= 3)
			{
				//service->proxyRegister();
			}
		}
	}
}