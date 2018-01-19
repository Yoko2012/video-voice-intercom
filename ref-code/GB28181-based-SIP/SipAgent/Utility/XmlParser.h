#ifndef _DDCPXMLPARSE_H
#define _DDCPXMLPARSE_H


#include <string>
#include "tinyxml.h"

const int max_title = 64;

#define XMLTITLE  "<?xml version=\"1.0\"?>\n"


/*ʵ�����¼�������:
1 xml�ļ���ʽͷ
2 ���һ�����ڵ�
3 ��ĳ���ڵ����һ���ӽڵ� ����: value��attributes��itemname...
4 ��������xml�ַ��� char*

5 ����-�õ�ĳ���ڵ��ֵ��attribute
*/


struct attributes
{
	std::string attrName;
	std::string attrVal;
};

class XmlParser
{
public:
	XmlParser(void);
	~XmlParser(void);

public:
	//��Ӹ��ڵ�
	TiXmlElement* AddRootNode(char* rootName);
	//��һ���ڵ��²������ӽڵ�
	TiXmlElement* InsertSubNode(char* parentNode, char* itemName, char* textValue);
	//���ƶ�node�ڵ��²����ӽڵ�
	TiXmlElement* InsertSubNode(TiXmlElement* parentNode, char* itemName, const char* textValue);

	TiXmlElement* GetSubNode(TiXmlElement* node, const char* item);
	TiXmlElement* GetNextSiblingNode(TiXmlElement* node, const char* value);
	//���Բ���
	void SetNodeAttributes(TiXmlElement* node, char* attributeItem, char* attributeTextValue);
	void GetNodeAttributes(TiXmlElement* node, std::string& attributes);

public:
	//����xml,�õ�����xml string
	void getWholeXml(char* xmlBuf, int size, int* actualLen);
  //�����õ�ĳ���ڵ��ֵ��attribute
	void  ParseNode(char* nodeName, std::string& textValue);
	void  ParseNode(TiXmlElement* node, std::string& textValue);
  //����һ���ַ���
	int   ParseDocumentXml(char* parseContent);

	void SetXmlTile(char* title);

	int SaveXmlFile(char* fileName);

	int LoadFile(char* fileName, int encoding = 1);

	TiXmlElement* GetRootNode() { if (m_document != NULL) return m_document->RootElement(); }


private:
	//��ĳ���ڵ�����Ȳ���ָ���ӽڵ�
	TiXmlElement* NodeFind(const char* value, TiXmlElement* node);

private:
	TiXmlDocument* m_document;
	TiXmlElement*  m_rootElement;

	char  m_xmlTitle[max_title];
};



#endif

