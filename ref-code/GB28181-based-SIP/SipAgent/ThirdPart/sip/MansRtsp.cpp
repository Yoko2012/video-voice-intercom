#include "MansRtsp.h"

#include <iostream>
#include <sstream>

MansRtsp::MansRtsp(void)
{
}

MansRtsp::~MansRtsp(void)
{
}


//����mansRtsp
void MansRtsp::ParseMansRtsp(const char* buf)
{

  //����������
	std::string str = buf;
	size_t firstPos = str.find_first_of("\r\n", 0);
	std::string firstLine = str.substr(0, firstPos);
	int p = firstLine.find(' ', 0);
	std::string m = firstLine.substr(0, p);

	strcpy(method, m.c_str());
 #if 0
	//����cseq
	size_t secondPos = str.find_first_of("\r\n", firstPos + 1);
	std::string secondLine = str.substr(firstPos, secondPos - firstPos);
	int p2 = secondLine.find(':', 0);
	std::string cseq = secondLine.substr(p2 + 1);



  size_t thirdPos = str.find_first_of("\r\n", secondPos + 1);
	if (thirdPos <= 0)
	{
		return ;
	}
	std::string thirdLine = str.substr(secondPos, thirdPos - secondPos);
	int p3 = thirdLine.find(':', 0);
	std::string head = thirdLine.substr(0, p3);
	std::string scale;
	if (head.compare("Scale") == 0)
	{
		scale = thirdLine.substr(p3 + 1);
	}
	else if (head.compare("Range") == 0)
	{
		std::string r = thirdLine.substr(p3 + 1);

	}
#endif

}