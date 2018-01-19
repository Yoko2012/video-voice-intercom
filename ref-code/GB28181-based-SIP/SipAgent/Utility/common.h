#ifndef _COMMON_H
#define _COMMON_H

#include <iostream>

#include <string>
#include <list>
using namespace std;


typedef union _ularge_int
{
	struct 
	{
		unsigned long wlowint;
		unsigned long whighint;
	};
	__int64 quadpart;

}ularge_int;



//common 
#define RET_IF_NULL(p, n) if (p == NULL) return n; 

#define SAFE_DEL(p) if (p != NULL) delete p;\
	                                 p = NULL;

#define VERIFY(exp) if (exp == -1) return -1;

//function
//18λ�����ַ����
struct AddrCode
{
	unsigned long province;
	unsigned long city;
	unsigned long county;
	unsigned long town;
	char  deviceCode[32];
	char  objCode[8];
	char  accessCode[4];  //��������
	int   parentType;     //���ڵ�����
};

//static function

//��֯��ddcp������18λ��ַ����
void ConstructAddrCode(AddrCode& code, std::string& addr);

//�����豸id��6λ���͸��ڵ����ͣ���ϳ�һ��6�����ȵ��ַ���
void CombineAddress(std::string addr, int parentType, std::string& outPutAddr); 

unsigned long getTickCount(void);

//��18λ��ַ������deviceId
int FromDeviceAddrToDevId(const char* address);


/*
ʱ���ַ���ת��time_t
 ʱ���ַ������磺2005-11-10T13:20:50
*/
time_t StrTimeToTime_t(const char* strTime);
void Time_tToStrTime(time_t t, std::string& strTime);


//
#if 0
SYSTEMTIME Time_tToSystemTime(time_t t);
time_t SystemTimeToTime_t(SYSTEMTIME& sysTm);
__int64 SystemTimeToTime_t2(SYSTEMTIME& sysTm);
#endif


//�и��ַ���
int SegString(string strInput, std::list<string>& strList, string sep);

#endif