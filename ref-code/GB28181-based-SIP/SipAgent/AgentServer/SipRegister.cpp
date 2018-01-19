#include "SipRegister.h"
#include "log4z.h"
#include "SipMgr.h"
#include "ATLComTime.h"
#include "../config/ProxyConfig.h"
#include "../GlobalCtrl.h"
#include "../Utility/TaskTimer.h"

//#define _CLIENT

#define EXPIRES 120

bool SipRegister::g_bRegisted = false;
RvSipRegClientHandle SipRegister::g_hRegClient = NULL;

SipRegister::SipRegister(CSipMgr* pServer)
{
	if (pServer != NULL)
	{
#ifdef _CLIENT
		pServer->Register(REGISTER, "", &(SipRegister::RegisterServiceMgr), this);
		pServer->Register(REGISTER, AUTHSUCCESS, &(SipRegister::RegisterServiceSuccess), this);
#endif
	}

	m_RegTimer = new TaskTimer(GLOBJ(gConfig)->getSip_regExpiresTm() - 10);
	memset(m_SysCheckDateTm, 0, sizeof(m_SysCheckDateTm));

}

SipRegister::~SipRegister(void)
{
	delete m_RegTimer;
	m_RegTimer = NULL;
}


int SipRegister::RegisterServiceStart()
{
#ifndef _CLIENT
	m_RegTimer->setTimerFun(RegisterProc, this);
	m_RegTimer->start();
#else


#endif

	return 0;
}


//����ע�����
/*
 1 ��ѯ���ݿ�
 2 �ж��Ƿ������ݿ��д���
 3 ��Ȩ
*/
void SipRegister::RegisterServiceMgr(SipMessage& msg, int& iresult, void* context)
{

/*
#ifndef _CLIENT
	char sql[128] = {0};
	sprintf_s(sql, 128, "select * from uac_list where uac_addr = '%s' and uac_port = %d", msg.from_ip(), msg.from_port());
	CppSQLite3Query sql_query = gDB.execQuery(sql);
	int numRows = sql_query.numFields();
	if (numRows <= 0)
	{
		//�������ݿ���
		iresult = SIP_NOTIMPLEMENT;
		return ;
	}

	//�Ƚ�type level 
	while( !sql_query.eof())
	{
		std::string password = sql_query.getStringField("uac_psw");
		std::string usrName = sql_query.getStringField("uac_name");
		if (usrName.compare(msg.auth_usr()) == 0)
		{
			msg.setAuth_password((char*)password.c_str());

			//��ע���uac��ӵ�������SipUacManager��
			int id = sql_query.getIntField("uac_id");
			std::string addr = sql_query.getStringField("uac_addr");
			int port = sql_query.getIntField("uac_port");
			gUacMgr.InsertUac(id, (char*)addr.c_str(), port);

			iresult = SIP_SUCCESS;
		}
		else
		{
			iresult = SIP_NOTIMPLEMENT;
		}

		return ;
		//sql_query.nextRow();
	}
  
	iresult = SIP_NOTIMPLEMENT;
#endif*/

	msg.setAuth_password("12345678");
	iresult = SIP_SUCCESS;

	_LOG(LOG_LEVEL_DEBUG, "�յ�REGISTER ����!");
}

//��Ȩ�ɹ��Ĵ���
void SipRegister::RegisterServiceSuccess(SipMessage& msg, int& iresult, void* context)
{
	std::string dateTm;
	time_t t;
	time(&t);
	//Time_tToStrTime(t, dateTm);
	dateTm += ".000";
	msg.setDateCheck(/*"Tue,15 Nov 1994 08:12:31 GMT"*/(char*)dateTm.c_str());
	_LOG(LOG_LEVEL_DEBUG, "%s ע��ɹ�", msg.from_user());
}


//�����ṩע��
int SipRegister::Register(int expires)
{
	int ret = -1;
	//��һ��ע��
	if (1/*!g_bRegisted || expires <= 0*/)
	{
		//ע��
		SipMessage msg;
		msg.setFrom(GLOBJ(gConfig)->getSiplocalUser(), GLOBJ(gConfig)->getSiplocalIp(), GLOBJ(gConfig)->getSiplocalPort());
		msg.setTo(GLOBJ(gConfig)->getSipremoteUser(), GLOBJ(gConfig)->getSipremoteIp(), GLOBJ(gConfig)->getSipremotePort());
		//msg.setTo(GLOBJ(gConfig)->getSiplocalUser(), GLOBJ(gConfig)->getSiplocalIp(), GLOBJ(gConfig)->getSiplocalPort());
		msg.setUri(GLOBJ(gConfig)->getSipremoteIp(), GLOBJ(gConfig)->getSipremotePort());
		msg.setExpires(expires);

		ret = CSipMgr::doSipRequest(REGISTER, msg);
		if (ret == SIP_OK)
		{
			if (msg.status_code() == SIP_SUCCESS)
			{
				if (expires > 0)
				{
					_LOG(LOG_LEVEL_DEBUG, "ע�ᵽsipServer-%s�ɹ�!", GLOBJ(gConfig)->getSipremoteIp());

					//��ʾ�ɹ�
					g_bRegisted = true;
					g_hRegClient = msg.m_hRegClient;
				}
				if (expires == 0)
				{
					_LOG(LOG_LEVEL_DEBUG, "ע���ɹ�!");
					return 0;
				}

				strcpy(m_SysCheckDateTm, msg.get_checkdate());
				std::string strTm = m_SysCheckDateTm;
				int pos = strTm.find('T');
				strTm.replace(pos, 1, " ");
				int pos2 = strTm.find('.');
				if (pos2 != std::string::npos)
					strTm.erase(pos2, strTm.length() - pos2);

				COleDateTime tm;
				SYSTEMTIME st;
				tm.ParseDateTime(strTm.c_str());
				tm.GetAsSystemTime(st);
				bool b = SetLocalTime(&st);

				/*
				m_RegTimer->setTimerFun(RegisterProc, this);
				m_RegTimer->start();*/

				//SetLocalTime();SetSystemTime
			}
		}
		else
		{
			if (ret == SIP_REQUEST_TIMEOUT)
				_LOG(LOG_LEVEL_DEBUG, "REGISTER ע�ᳬʱ!");
		}
	}
	else
	{
		RvStatus rv;
		if (g_hRegClient != NULL)
		{
			rv = RvSipRegClientRegister(g_hRegClient);
			if (rv == RV_OK)
			{
				_LOG(LOG_LEVEL_DEBUG, "ע�ᵽsipServer-%s�ɹ�!", GLOBJ(gConfig)->getSipremoteIp());
			}
		}

		ret = 0;
	}
	

	return ret;
}


void SipRegister::RegisterProc(void* param)
{
	SipRegister* pthis = (SipRegister*)param;
	int ret = pthis->Register(GLOBJ(gConfig)->getSip_regExpiresTm());

}