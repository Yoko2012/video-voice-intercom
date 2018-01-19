#pragma once
enum REGISTER_STATUE
{
	SIP_UNREGISTERED,
	SIP_REGISTERED
};

enum SIPAGENT_TYPE
{
	AGENT_SMS = 1,  //媒体服务器
	AGENT_DVR,      //视频发送者
	AGENT_SINK,     //视频接收者
	AGENT_GA,          //上级域
	AGENT_GS,          //本级域
};

enum SIPREQUST_STATUS
{
	STATUS_IDLE=0,
	STATUS_SEND_INVITE_NOSDP,
	STATUS_SEND_INVITE_SDP,
	STATUS_SEND_200OK,
	STATUS_SEND_ACK,
	STATUS_RECE_INVITE_NOSDP,
	STATUS_RECE_INVITE_SDP,
	STATUS_RECE_200OK,
	STATUS_RECE_ACK,
};

enum  SIPSESSION_TYPE
{
   SESSION_REALTIME_VIDEO = 1, //实时视频
   SESSION_RECODER_PLAYVACK,   //历史视频回放
   SESSION_RECODER_SEARCH,   //历史视频检索
};