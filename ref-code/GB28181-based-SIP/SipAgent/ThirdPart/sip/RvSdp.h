#pragma once
//SDP Э��
#include "RvSipStackTypes.h"
#include "rvsdpmsg.h"
#include "rvsdpobjs.h"
#include "rvsdpprsaux.h"
#include "rvsdpinit.h"
#include "rvsdpenc.h"

#include "sipDef.h"

#define ATTRIBUTESIZE 16

class RvSdp
{
public:
	RvSdp(void);
	~RvSdp(void);

	  
  struct other_field
  {
	  char tag;
	  char val[32];
  };

public:
	const RvSdp& operator=(const RvSdp& sdp);
	int ParseSdpInfo(char* sdpBuffer, int len);

	//attribute
	inline char* getV() { return vision; }
	inline char* getS() { return sName; }
	inline char* getT() { return t; }
	void getO(Origin* o);
	void getM(mediaDescr* m);
	void getA(attribute** a);
	void getC(cconnect* c );
	inline char* getU() { return uri; }

	void setO_user(char* user);
  
	//����ý�������ն˵�ip�Ͷ˿�
	void SetNetAddressAndPort(char* netAddr, int port);
	//��ȡ�µ�sdp��
	void GetSdpMsgBuf(char* pBuf, int len);

	void GetSdpMsgHandle(RvSdpMsg* sdp);

	void setOtherField(char tag, const char* val)
	{
		otherfield.tag = tag;
		strcpy_s(otherfield.val, sizeof(otherfield.val), val);
		//RvSdpOther yother;
		rvSdpMsgAddOther(&m_pSdpMsg, tag, otherfield.val);
	}

private:
	RvStatus EncodeSdpMsgToBuffer(RvSdpMsg *pMsg,	int bufLen,	char *encodedBuf);
  RvStatus CreateSdpMsgFromEncodedBuffer(char *pBuffer, int *pBufSize, RvSdpMsg* pMsg);

	sdpMediaType TransferMediaType(RvSdpMediaType ty);
	sdpProtocolType TransferProtocolType(RvSdpProtocol);

	//ȡ��unregular�е��ֶ�
	void ObtainUnregularHead(char* pMsg);
public:
	//v �汾
	char vision[8];
	//o
	Origin osIdentifier;
	//s
	char sName[32];
	//u
	char uri[32];
	//c
	cconnect cInfo;
	//t
	char t[32];
	//m
	mediaDescr mNameAndAddr;
	//a
  attribute attributeLines[ATTRIBUTESIZE];
  //other
  other_field otherfield;
	

  //Rv_sdp Message
	RvSdpMsg m_pSdpMsg;
	//content
	char* m_pContent;

	//
	HRPOOL m_sdpPool;
};
