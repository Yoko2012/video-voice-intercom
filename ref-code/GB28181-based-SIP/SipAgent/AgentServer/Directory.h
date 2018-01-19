#pragma once

#include "SipMgr.h"
#include "../GlobalCtrl.h"
#include "SipHandleBase.h"
#include "GBStandard.h"
#include "BcAdaptor/adaptorCommon.h"

#include <list>

struct AnswerArgs;

//Ŀ¼������


//Ŀ¼��Ϣ�ṹ��
struct DirectoryNodeInfo
{
	int type;         //�豸����
	char DeviceID[32];     //id
	char Name[64];        //����
	char Manufacturer[32]; //�豸����
	char Model[16];       //�豸�ͺ�
	char Owner[16];       //�豸����
	char CivilCode[16];   //��������
	char Block[16];       //����--��ѡ
	char Address[16];     //��װ��ַ
	int Parental;         //�Ƿ������豸
	char ParentID[32];    //���豸ID
	int SafetyWay;        //���ȫģʽ--��ѡ
	int RegisterWay;      //ע�᷽ʽ1�� 2�� 3��
	char CertNum[16];     //֤�����к�--��ѡ
	int Certifiable;      //֤����Ч��ʾ--��ѡ
	int ErrCode;          //֤����Чԭ����--��ѡ
	char EndTime[16];     //֤����ֹ��Ч��--��ѡ
	int Secrecy;          //��������
	char IPAddress[24];   //�豸ϵͳip��ַ--��ѡ
	int Port;             //�˿�--��ѡ
	char Password[32];    //�豸����--��ѡ
	char Status[4];           //�豸״̬
	double Longitude;     //��γ��--��ѡ
	double Latitude;      //��γ��--��ѡ


	DirectoryNodeInfo()
	{
		memset(DeviceID, 0, sizeof(DeviceID));
		memset(Name, 0, sizeof(Name));
		strcpy_s(Manufacturer, sizeof(Manufacturer), "BlueSky");
		strcpy_s(Model, sizeof(Model), "bc2.6.x");
		
		strcpy_s(Owner, sizeof(Owner), "BlueSky");
		memset(CivilCode, 0, sizeof(CivilCode));
		strcpy_s(Block, sizeof(Block), "alarm1");
		strcpy_s(Address, sizeof(Address), "bcenter");
		Parental = 1;
		memset(ParentID, 0, sizeof(ParentID));
		SafetyWay = 0;
		RegisterWay = 1;
		Secrecy = 0;
		strcpy(Status, "ON");

	}

	const DirectoryNodeInfo& operator=(const DirectoryNodeInfo& node)
	{
		if (this == &node)
		{
			return *this;
		}

		strcpy_s(DeviceID, sizeof(DeviceID), node.DeviceID);
		strcpy_s(Name, sizeof(Name), node.Name);
		strcpy_s(Manufacturer, sizeof(Manufacturer), node.Manufacturer);
		strcpy_s(Model, sizeof(Model), node.Model);
		strcpy_s(Owner, sizeof(Owner), node.Owner);
		strcpy_s(CivilCode, sizeof(CivilCode), node.CivilCode);
		strcpy_s(Block, sizeof(Block), node.Block);
		strcpy_s(Address, sizeof(Address), node.Address);

		Parental = node.Parental;
		strcpy_s(ParentID, sizeof(ParentID), node.ParentID);
		SafetyWay = node.SafetyWay;
		RegisterWay = node.RegisterWay;
		Secrecy = node.Secrecy;
		strcpy_s(Status, sizeof(Status), node.Status);

		return *this;
	}

};

//ddcpЭ�����Ŀ¼���ͽṹ����
struct DirCatalog
{
	std::string via;
	std::string parentCode;
	int type;
	int  SubNum;
	std::list<DirectoryNodeInfo> Sub;//[MAX_NODE_NUM];

	DirCatalog()
	{
		via = "0";
		SubNum = 0;
		type = 0;
		parentCode = "130202";
	}
	~DirCatalog()
	{
		Sub.clear();
	}

	void clear()
	{
		via = "0";
		type = 0;
		SubNum  = 0;
		Sub.clear();
		parentCode.clear();
	}
};

typedef std::list<DirectoryNodeInfo>::iterator DirIter;


//--------------------------------


class Directory : SipHandleBase
{
public:
	Directory(CSipMgr* pServer);
	~Directory(void);

public:
	static void DirectoryQuery(SipMessage& msg, int& iresult, void* context);

		//Ŀ¼����
	static void CatelogSubscribeCB(SipMessage& msg, int& iresult, void* appData);

	static void RpcQueryDir(AnswerArgs* args);
private:
	static void pushRootDomain(int sn, const char* usr, const char* ip, const int port);
	static int SendSipDirMsg(const char* sipBody, const char* touser, const char* toip, int toport);
	static void ParseXmlRpcDir(const char* xmlRpcDir, int reqLen, DirCatalog& dirList);
	static void pushDevDirectory(DirCatalog& dir, int sn, const char* usr, const char* ip, const int port);
	static void recurvePushDir(DirCatalog& dir, int sn, const char* usr, const char* ip, const int port);

	//����ֿ����ĵ�Ŀ¼
	static void getSubCenterDir(int sn, const char* usr, const char* ip, const int port);

	static void constructMANSCDPXml(DirCatalog& dirSet, int SN, int begin, int count, char* sendData, int* len);
	static void DevListNotify(DirCatalog& dirSet, int SN, int begin, int count, char* sendData, int* len);

public:
	//
	void autoDirNotify();

public:
	//test record file
	static void constructRecordFile(int SN, char* buf, int len);
};
