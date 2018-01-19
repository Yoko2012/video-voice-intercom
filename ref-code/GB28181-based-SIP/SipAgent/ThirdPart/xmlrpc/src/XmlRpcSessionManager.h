/** $Id: //depot/NVS/v2.1/xmlrpc/src/XmlRpcSessionManager.h#2 $$DateTime: 2008/11/11 16:56:21 $
 *  @file XmlRpcSessionManager.h
 *  @brief A TCP connection status tracking manager for login/logout
 *  @version 0.5
 *  @author Tyhoon<tanf@bstar.com.cn> 
 *  @date 2006-06-22    Create it
 */
/************************************************************
 *  @note
    Copyright 2005, BeiJing Bluestar Corporation, Limited	
 			ALL RIGHTS RESERVED			
 Permission is hereby granted to licensees of BeiJing Bluestar, Inc.	 products to use or abstract this computer program for the sole	 purpose of implementing a product based on BeiJing Bluestar, Inc. products. No other rights to reproduce, use, or disseminate this computer program,whether in part or in whole, are granted. BeiJing Bluestar, Inc. makes no representation or warranties with respect to the performance of this computer program, and specifically disclaims any responsibility for any damages, special or consequential, connected with the use of this program.
 For details, see http://www.bstar.com.cn/ 
***********************************************************/

#ifndef _XMLRPCSESSIONMANAGER_H
#define _XMLRPCSESSIONMANAGER_H
//
// XmlRpc++ Copyright (c) 2006 by Tan Feng
//
#if defined(_MSC_VER)
# pragma warning(disable:4786)    // identifier was truncated in debug info
#endif


#include "XmlRpcUtil.h"

#ifndef MAKEDEPEND
# include <list>
# include <vector>

#if defined(_WINDOWS)
#include <winsock2.h>
#else
#include <pthread.h>
#endif

#endif

namespace XmlRpc {

  struct SessionItem{};

  /*
  struct SessionContext 
  {
      std::string clientAddr;
      unsigned short clientPort;
      std::string serverAddr;
      unsigned short serverPort;
      bool ssl;
      bool authed;
      time_t time;
      time_t timestamp;
      SessionItem* data;
  };
  */

  enum SessionEventType 
  { 
    onSessionOpen,   // ���пͻ������ӳɹ�����¼�
    onSessionClose,  // �Ͽ�����
    onSessionLogin,  // �ͻ���ע��session����¼�,ָ����ɵ�¼��.
    onSessionLogout, // ע��
    onSessionTouch   // ��������Ч,���迪��,��Ҫ�޸� XmlRpcServerConnection::executeMethod()�ڵĴ���
  }; 

  typedef void (* SessionEventHandler)(int id);
   
  class XmlRpcSessionManager {
  public:
    //! Constructor
    XmlRpcSessionManager();
    ~XmlRpcSessionManager();

    // ����/ɾ��һ����sessionԪ��,ָ��socket fd.
    void open(int fd);
    bool close(int fd);

    // bool touch(int fd);
    // ��¼/ע��ĳsession
    bool login(int fd, int id, SessionItem* data /* �ò���ʧЧ */); 
    bool logout(int fd, int id); 

    // ͨ��socket fd��ȡ��¼��֤���session id.
    // �緵��-1��ʾ������,����δ������֤.
    int getSessionId(int fd);

		// ͨ��socket ��ȡ��¼��֤���client ip.
		bool getIpByFd(int fd, std::string& ip); // modified_by_cp

		// ͨ��SessionID ��ȡ��¼��֤���client ip.
		bool getIpById(int id, std::string& ip); // modified_by_cp

		// �������session��ip���ڷ���udp������Ϣ���ͻ���
		bool GetAllSessionIP(std::list<std::string> &ip_list); // modified_by_cp

    // ���ĳsession id �Ƿ��ѵ�¼��֤.
    bool checkSessionId(int id);
      
    // ���socket fd�ĶԶ��Ƿ��Ǳ�������.
    bool isLocal(int fd); 
    
    // bool getContext(int id, SessionContext& ctx); 
    void list(std::vector<int>& ls);
    int  id2fd(int id);
    int  fd2id(int fd);

    // ע��ĳ�¼��Ļص�
    void catchEvent(SessionEventType event, SessionEventHandler handler);

    // void dump();
    void clear(); 

   private:
    struct Session
    {
      Session():fd(-1),id(0),authed(false),data(NULL){}
      Session(int fd):fd(fd),id(0),authed(false),data(NULL){}
      int fd;             // socket fd
      int id;             // ��¼���session��
      bool authed;        // �Ƿ�����ɵ�¼����֤
			std::string ip; // client ip // modified_by_cp
      // time_t time;
      // time_t timestamp;
      SessionItem* data;  // ����
    };
    struct SessionEvent
    { 
      SessionEvent(SessionEventType e, SessionEventHandler h): event(e), handler(h){}
      SessionEventType event;
      SessionEventHandler handler;
    };

    // A list of sessions to monitor
    typedef std::list< Session* > SessionList; 
    // A list of callback handlers on session close
    typedef std::vector< SessionEvent > EventList; 

    // Sessions being monitored
    SessionList _sessions;

#if defined(_WINDOWS)
    // winƽ̨����δʵ��
    CRITICAL_SECTION _sessionsLock;
    class SessionLock
    {
    public:
      SessionLock(LPCRITICAL_SECTION lpcs ):_lpcs(lpcs) {EnterCriticalSection(_lpcs);}
      ~SessionLock(){LeaveCriticalSection(_lpcs);}
    private:
      LPCRITICAL_SECTION _lpcs;
    };

#else
    pthread_mutex_t _sessionsLock;

    // �������ͷŷ�װ��
    class SessionLock
    {
    public:
      SessionLock(pthread_mutex_t* mutex) : ml(mutex) { XmlRpcUtil::getLock(ml); }
      ~SessionLock() { XmlRpcUtil::freeLock(ml); }

    private:
      pthread_mutex_t *ml;
    };
#endif



    // Session events being monitored
    EventList _events;

    Session* find(int fd);      // ͨ��socket fd����
    Session* findById(int id);  // ͨ��sessionId����
    void doEvent(SessionEventType event, int id);


 }; //class SessionManager

} // namespace XmlRpc

#endif  // _XMLRPCSSIONMANAGER_H_
