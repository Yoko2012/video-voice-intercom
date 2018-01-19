#pragma once

class RpcCmd;
class CSipMgr;
class ProxyConfig;
class BcAdaptor;


#include "ThreadPool.h"
//
class GlobalCtrl;
#define GLOBJ(obj) GlobalCtrl::instance()->obj

class GlobalCtrl
{
public:
	static GlobalCtrl* instance();
  
	//��ʼ��ȫ�ֶ���
	int init();

	//����sip-server
	static unsigned int __stdcall sipRunProc(void* context);
	static bool gSipInit;
public:
	//xmlrpc �������
	RpcCmd* gRpcExcutor;
	//sip�ϲ�Ӧ�ù���
	CSipMgr* gSipServer;
	//�����ļ�
	ProxyConfig* gConfig;
	//thread pool
	base::ThreadPool* gThreadPool;

	//adaptor
	BcAdaptor* gAdaptor;


private:
	GlobalCtrl(void){}
	~GlobalCtrl(void);
	GlobalCtrl(const GlobalCtrl& global);
	const GlobalCtrl& operator=(const GlobalCtrl& global);

private:
	static GlobalCtrl* ms_pInstace;


	class Guard //
	{
	public:
		~Guard()
		{
			delete GlobalCtrl::ms_pInstace;
			GlobalCtrl::ms_pInstace = NULL;
		}
	};

	static Guard Garbo; 

};
