#ifndef _PROXYCONFIG_H
#define _PROXYCONFIG_H

#include "../Utility/XmlParser.h"

#include "HostDomain.h"

#include <list>

/*
1 ���������ļ� proxy_config.xml
2 ��ȡ����ģ��ȫ��������洢���ڴ���
3 �Ը���ģ���ṩ �� д������ӿ�
*/
/*
<proxy_cfg>
  <module_alarm>
	 <option>bAccept</option>
	</module_alarm>
	<module_sip>
	</module_sip>
</proxy_cfg>
*/

class ProxyConfig
{
public:
	ProxyConfig(void);
	~ProxyConfig(void);

public:
	//��ʼ�������ļ������������ļ�
	int InitCfg();

	//���������ļ�
	int LoadCfg();

public:
	XmlParser* m_xmlCfg;

	//ģ��
	//main
	//center server ip
	std::string main_serverIp;
	//center server port
	int  main_serverPort;
	//��½ server ��user name
	std::string main_loginName;
	//��½ server ��password
	std::string main_loginPassword;
	//������ʱ���Ƿ�����Ŀ¼����
	int main_IsReSendDir;

	//����������
	std::string domain_name;

	//live �ֳ�������ý����Ϣ����
	//live streamer ��ip �Ͷ˿�
	std::string live_streamerIp;
	int live_streamer_cmd_port;
	int live_streamer_heartbeat_port;

	//sip config
	//����sip���պͷ��Ͷ˿�
	int  sip_localPort;     //����sip ip�Ͷ˿�
	std::string sip_localIp;
	int  sip_remotePort;  //Զ��sip�˵�ip�Ͷ˿�
	std::string sip_remoteIp;
	std::string sip_localUser;  //����user
	std::string sip_remoteUser;  //Զ��user
	//��Ȩ
	std::string sip_auth_User;    //��Ȩ �û���������
	std::string sip_auth_Password;
	std::string sip_auth_realm; 
	std::string sip_auth_nonce;
	std::string sip_auth_sdomain;
	std::string sip_auth_opaque;
	//ע���ʱ�������ʱ����
	int sip_heartbeat;
	int sip_RegExpires;

  //sip ����
	std::list<AbsDomain> m_SipDomainList;

	//alarm
	std::string alarm_serverIp;
	int alarm_serverPort;
	int alarm_localReceivePort;

	//playback
	std::string playback_ip;
	int playback_port;
	std::string playback_rtsp_addr;//�ط�ģ���rtsp ip��port����֯��url




public:
	inline char* getServerIp() {return (char*)main_serverIp.c_str(); }
	inline int   getServerPort() {return main_serverPort; }
	inline char* getLoginName() {return (char*)main_loginName.c_str(); }
	inline char* getLoginPassword() {return (char*)main_loginPassword.c_str(); }
	inline bool  IsReSendDir()
	{
		bool bsend = (main_IsReSendDir == 0 ? false : true);
		return bsend;
	}

	//live 
	inline char* getStreamerIp() { return (char*)live_streamerIp.c_str(); }
	inline int   getStreamerCmdPort() { return live_streamer_cmd_port; }
	inline int   getStreamerHeartBeatPort() { return live_streamer_heartbeat_port; }
	//inline int   getStreamerHeartBeatPort() { return ;}

	//sip
	inline int   getSiplocalPort() {return sip_localPort; }
	inline char* getSiplocalIp() { return (char*)sip_localIp.c_str(); }
	inline char* getSipremoteIp() { return (char*)sip_remoteIp.c_str(); }
	inline int   getSipremotePort() { return sip_remotePort; }
	inline char* getSiplocalUser() { return (char*)sip_localUser.c_str(); }
	inline char* getSipremoteUser() { return (char*)sip_remoteUser.c_str(); }
	inline char* getSip_authUser() { return (char*)sip_auth_User.c_str(); }
	inline char* getSip_authPassword() { return (char*)sip_auth_Password.c_str(); }
	inline char* getSip_auth_realm() { return (char*)sip_auth_realm.c_str(); } 
	inline char* getSip_authnonce() { return (char*)sip_auth_nonce.c_str(); }
	inline char* getSip_authsdomain() { return (char*)sip_auth_sdomain.c_str(); }
	inline char* getSip_authopaque() { return (char*)sip_auth_opaque.c_str(); }

	inline char* getLocalDomainName() {return (char*)domain_name.c_str(); }

	inline int   getSip_heartBeatTm() { return sip_heartbeat; }
	inline int   getSip_regExpiresTm() { return sip_RegExpires; }

	//alarm
	inline char* getAlarmServerIp() { return (char*)alarm_serverIp.c_str(); }
	inline int getAlarmLocalPort() { return alarm_localReceivePort; }
	inline int getAlarmServerPort() { return alarm_serverPort; }

	//playback
	inline char* getPlaybackip() { return (char*)playback_ip.c_str(); }
	inline int   getPlaybackPort() { return playback_port; }
	inline char* getPlaybackRtspAddr() { return (char*)playback_rtsp_addr.c_str(); }

public:
	//��ʼ����ģ��
	void defaultMain();
	void parseMain();
	//��ʼ���ֳ�ģ��
	void defaultLive();
	void parseLive();
	void liveInit();
	//sip
	void defaultSip();
	void parseSip();
	void sipInit();
	//sip_domain
	void defaultSipDomain();
	void parseSipDomain();
	void sipDomainInit();
	//alarm
	void defaultAlarm();
	void parseAlarm();

	//playback
	void defaultPlayback();
	void parsePlayback();

};


#endif

