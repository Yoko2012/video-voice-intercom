#pragma once
#include "SipClientApent.h"

class CSipSession
{
public:
	CSipSession(void);
	CSipSession(CSipClientApent *proxy,CSipClientApent *sms,CSipClientApent *sender,CSipClientApent *receiver,int seesiontype);
	~CSipSession(void);

	int check_session(string id,int cid);//����Ƿ����ڱ��λỰ����Ϣ
	void insert_call(int cid);
	void remove_call(int cid);

	//ʵʱ��Ƶ�Ự����
    int handle_invite(eXosip_t *context_eXosip,eXosip_event_t *evt);
	int handle_answered(eXosip_t *context_eXosip,eXosip_event_t *evt);
	int handle_ack(eXosip_t *context_eXosip,eXosip_event_t *evt);
	int handle_bye(eXosip_t *context_eXosip,eXosip_event_t *evt);

private:
    CSipClientApent *sipagent_proxy_; //������
	CSipClientApent *sipagent_sms_;   //ý�������
	CSipClientApent *sipagent_sender_; //ý�������
	CSipClientApent *sipagent_receiver_; //ý�巢����

	ACE_Recursive_Thread_Mutex    sipcall_lock_;
	vector<int>           v_callid_;

	int   session_type_;  //�Ự����

	int   sender_cid_;
	int   sender_did_;
	int   sender_tid_;

	int   recevier_cid_;
	int   recevier_did_;
	int   recevier_tid_;

	int   sms_cid_;
	int   sms_did_;
	int   sms_tid_;
};
