#pragma once

/*
��� ��ȫ������Ƶ�������ϵͳ GB28181�й���MANSRTSP������

PLAY MANSRTSP/1.0
CSeq:5
Scale:1.0
Range:npt=196-

PAUSE MANSRTSP/1.0
CSeq:5
Range:npt=123(��ѡ)

TEARDOWN MANSRTSP/1.0
CSeq:5



����MansRtsp
*/
class MansRtsp
{
public:
	MansRtsp(void);
	~MansRtsp(void);

public:
	void ParseMansRtsp(const char* buf);

public:
	char method[16];
	int cseq;
	double scale;
	int nptStart;
	int nptEnd;
};
