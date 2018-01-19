#ifndef _ERROR_H
#define _ERROR_H


enum errorCode
{
	SIP_SUCCESS = 200,
	SIP_NOTIMPLEMENT = 501,    //���󷽷���֧��
	SIP_NOTAUTHENTICATION = 401, //REGISTER δ��Ȩ

	//�ϲ�ҵ���ִ�д���
	SIP_COUNT_LIMIT  = 701,  // ע������: Ŀ��������Ԫ�ϵĿ�ע�ᵥԪ�Ѵﵽ���ֵ
	SIP_VALID_PROXY  = 702,  // δ��Ȩ��������Ԫ: Դ������Ԫע��Ǽ���Ϣ������ϵͳ���õĽ�����������
	SIP_LACK_INFO    = 713,  // DDCP��Ϣ��ȫ: DDCP�е���Ϣ��ȫ����ѡ�ֶβ����ڣ�

	SIP_BADREQUEST   = 400,

	//sip ִ���ڼ�Ĵ��󷵻�ֵ����
	SIP_OK  = 0,                 //ִ�гɹ�
	SIP_REQUEST_FAILED = -1,     //sip����ִ��ʧ��
	SIP_REQUEST_TIMEOUT = -100,  //sip����ʱ
	SIP_INIT_ERROR     = -2,     //sip���ʼ��ʧ��

};


typedef struct _tagSipError
{
	errorCode code;
	char*     explain;
}SipError;


//SipError er[] = 
//{
//};



#endif