#ifndef _PLAYBACKDEF_H
#define _PLAYBACKDEF_H

#include <list>


// file type������DB33-3-A.1
// 0��ʾȫ������, 1��ʾ��ʱ¼��, 2��ʾ�ƶ����¼��, 3��ʾ����¼��, 4��ʾ�ֶ�¼��, 5-9����
enum PlaybackFileType
{
	recordtype_all = 0,
	recordtype_time = 1,
	recordtype_move = 2,
	recordtype_alarm = 3,
	recordtype_manual = 4,
  //5-9 extend
};

//�طŵ��豸��Ϣ
typedef struct 
{
	int deviceId;
	int nChannel;
	int disk;
	char vendor[32];
	int filetype;
	char devAddr[32];
	char proxyAddr[32];
	int cmdPort;
	int dataPort;
	int devCmdPort;
	char devUsr[32];
	char devPassword[32];
}DeviceInfo;


//ͳһ�ط��ļ�����
typedef struct tagPLAYBACK_RECORDFILE_INFO
{
	unsigned int ch; //ͨ����
	char filename[256]; //�ļ���
	unsigned int size; //�ļ���С
	__int64 starttime;  //�ļ���ʼʱ��
	__int64 endtime;    //�ļ�����ʱ��
	unsigned char nfiletype;  //�ļ�����
	unsigned char ncluster;   //����

	tagPLAYBACK_RECORDFILE_INFO& operator=(const tagPLAYBACK_RECORDFILE_INFO& info)
	{
		if (&info == this)
		{
			return *this;
		}
		ch = info.ch;
		size = info.size;
		starttime = info.starttime;
		endtime = info.endtime;
		nfiletype = info.nfiletype;
		ncluster = info.ncluster;
		strcpy_s(filename, sizeof(filename), info.filename);

		return *this;
	}

}PLAYBACK_RECORDFILE_INFO, *LPPLAYBACK_RECORDFILE_INFO;


//�ļ��б�
struct playback_filelist
{
	unsigned int num; //�ļ�����Ŀ
	std::list<PLAYBACK_RECORDFILE_INFO> filelist;

};


//�ط���ʷ�ļ������ṹ��
struct DevFileSearchInfo
{
	int deviceId;  //�豸��ַ
	char privilege[8];//Ȩ�޹�����
	int fileType; //�ļ�����
	int fromIndex; //��ʼ����
	int toIndex;//��ֹ����
	__int64 beginTm; //��ʼʱ��
	__int64 endTm;  //��ֹʱ��
};

struct FileVodUrlRequestInfo
{
	char privilege[8];//Ȩ�޹�����

};

#endif