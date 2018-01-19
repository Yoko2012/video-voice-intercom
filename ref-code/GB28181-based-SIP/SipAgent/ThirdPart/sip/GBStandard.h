#pragma once

/*
  ��ѭGB28181��׼�����
	GB T 28181--2011��ȫ������Ƶ�������ϵͳ ��Ϣ���䡢���������Ƽ���Ҫ��.pdf
*/

#include"..\tinyxml\tinyxml.h"

#include <string>

class CGBStandard
{
public:
	CGBStandard(void);
	~CGBStandard(void);

public:
	static TiXmlElement* NodeFind(char* value, TiXmlElement* node);
	static void GBParseAppCmdTypeStr(const char* msg, std::string& strCmd);

	static void GBParseSdpCmdTypeStr(const char* msg, std::string& strCmd);
	static void GBParseMansRtspCmdTypeStr(const char* msg, std::string& strCmd);
};



void exchangeType(int* bsType, int* gbType);

//
//�������ϵͳ����=���ı���+��ҵ����+���ͱ���+���
//���ı���8λ��34000001
//��ҵ����2λ��00
//���ͱ���3λ��118
//��ű���7λ��0000001

//���ͱ���ö��
enum DevTypeCode
{
	Dvr_Code = 111, //DVR
	VideoServer_Code = 112, //��Ƶ������
	Encoder_Code = 113, //encoder
	Decoder_Code = 114, //decoder
	AlarmDev_Code = 117, //��������
	NVR_Code = 118, //nvr

	Camera_Code = 131, //camera
	Ipc_Code = 132, //ipc
	VGA_Code = 133, //vga
	AlarmInput_Code = 134, //alarm input
	AlarmOutput_Code = 135, //alarm output

	CenterServer_Code = 200, //����������Ʒ���������
	//
	District_Code = 202,
	Site_Code = 203
};



namespace GBSysCode
{
	void constructSysCode(std::string& code, const char* orga, const char* business, const char* type, const char* id);
	void getDevTypeAndId(const char* code, int* type, int* id);

	std::string gb28181Code(std::string& civilcode, int nodeType, int nodeId);
}



//GB28181 
//device control cmd request parse
namespace GBDevControlReq
{
  //information
	struct DevControl
	{
		std::string devId;
		int m_sn;
		std::string m_controlCmd;
		std::string m_controlType;
	};

	int DirQueryParse(const char* req, DevControl* ctrl);


}

//GB28181 
//device control cmd responce parse
class GBDevControlResp
{

};