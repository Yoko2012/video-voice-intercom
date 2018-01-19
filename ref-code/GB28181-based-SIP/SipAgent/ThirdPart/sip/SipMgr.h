#pragma once

/*
	ȫ��sip�����sip�⹹�죬����
*/

#include "SipMessage.h"
#include "SipModule.h"

//headers
#include <iostream>
#include <sstream>

#include "md5driver.hpp"

#include "HostDomain.h"
#include "ThreadPool.h"


///////////////////////////////////
class CSipMgr
{
public:
	CSipMgr();
	~CSipMgr(void);

public:
	//init
	int initSipStack(int serverPort);
	//sip stack initialize
	bool sipStackInitialize();

	//start
	int startupSipServer();

	//sip method register
	int Register(const char* request, const char* functionCmdType, SipMethod method, void* data);

	//ִ�з�������
	static int doSipRequest(const char* request, SipMessage& msg);

	//auth
	int setAuthentication(char* user, char* psw, char* realm, char* nonce, char* qop, char* opaque, char* algorithm, char* sdomain);

	static void __stdcall print2screen(void* context, RvSipLogFilters filter, const RvChar* formattedText);

	static void SetLogOn(bool blog) { g_LogOn = blog;}
	void RegisterHostDomain(char* host, char* user, int port, char* domain);
public:
	//sip�������
	static RvSipStackHandle      g_hStackMgr;
	static RvSipTranscMgrHandle g_hTranscMgr;
	static RV_LOG_Handle         g_hLog;
	static HRPOOL                g_appPool;
	static RvSipCallLegMgrHandle g_hCallLegMgr;
	static RvSipSubsMgrHandle g_hSubsMgr;
	static RvSipRegClientMgrHandle g_hRegClientMgr;
	static RvBool bSendUnsubscribe;
	static RvSipAuthenticatorHandle g_hAuthenticatorMgr;
	static RvInt32              g_AuthCounter;

	//port
	static int g_serverPort;

	//thread pool
	static base::ThreadPool g_threadPool;

	//host
	static HostDomain g_HostMomainResolve;
  
	//auth
	static Authentication g_Athorization;

	//log 
	static bool g_LogOn;

};
