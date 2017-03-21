#include "CfgFromXml.h"
#include "tinyxml/tinyxml.h"
#include "err.h"
#include "general.h"

CCfgFromXml::CCfgFromXml(void) : m_pDocument(NULL), m_pCurNode(NULL), m_pCurElement(NULL)
{
}

CCfgFromXml::~CCfgFromXml(void)
{
	if(NULL == m_pDocument)
		return ;

	delete m_pDocument;
	m_pDocument = NULL;
}

bool CCfgFromXml::LoadXml(const char* pszXmlFile)
{
	m_pDocument = new TiXmlDocument(pszXmlFile);
	alloc_assert(m_pDocument);
	if(false == m_pDocument->LoadFile(TIXML_ENCODING_UTF8))
	{
		delete m_pDocument;
		m_pDocument = NULL;
		return false;
	}

	m_pCurElement = m_pDocument->RootElement();

	return true;
}

bool CCfgFromXml::LoadPwdXml(const char* pszXmlFile)
{
	//¼ÓÔØÅäÖÃÎÄ¼þ
	char szPath[260] = {0};
	get_cur_directory(szPath, 260);
	strcat(szPath, "/");
	strcat(szPath, pszXmlFile);

	m_pDocument = new TiXmlDocument(pszXmlFile);
	alloc_assert(m_pDocument);
	if(false == m_pDocument->LoadFile(TIXML_ENCODING_UTF8))
	{
		delete m_pDocument;
		m_pDocument = NULL;
		return false;
	}

	m_pCurElement = m_pDocument->RootElement();

	return true;
}

bool CCfgFromXml::MoveRootElement()
{
	m_pCurElement = m_pDocument->RootElement();
	return NULL != m_pCurElement;
}

bool CCfgFromXml::MoveToElement(const char* pszKey)
{
	if(m_pCurElement == NULL)  
		return false;

	TiXmlNode* pNode  = NULL;

	for(pNode = m_pCurElement->FirstChildElement(); pNode != NULL ; pNode = pNode->NextSiblingElement())  
	{
		switch(pNode->Type())
		{
		case TiXmlText::TINYXML_ELEMENT:
			{
				std::string strValue = pNode->Value();
				if(strValue != pszKey)
					continue;

				m_pCurElement = pNode->ToElement();
				m_pCurNode = pNode;
				return true;
			}
			break;
		default:
			break;
		}
	}

	return false;
}

bool CCfgFromXml::GetAttrValue(const char* pszKey, const char* pszAttr, std::string& strValue)
{
	strValue.clear();

	if(m_pCurElement == NULL)
		return false;

	std::string strTmp;
	TiXmlNode* pNode = m_pCurNode->FirstChildElement();

	for( ; pNode != NULL; pNode = pNode->NextSiblingElement())
	{
		if(strcasecmp(pNode->Value(), pszKey) != 0)
			continue;

		TiXmlAttribute* pAttr = pNode->ToElement()->FirstAttribute();
		for( ; pAttr != NULL; pAttr = pAttr->Next())
		{
			if(strcasecmp(pAttr->Name(), pszAttr) != 0)
				continue;

			strValue = pAttr->Value();
			break;
		}

		break;
	}

	return !strValue.empty();
}

bool CCfgFromXml::GetTextValue(const char* pszKey, std::string& strValue)
{
	strValue.clear();

	if(NULL == m_pCurNode->FirstChildElement())
	{
		TiXmlNode* pNode = m_pCurNode->FirstChild();
		if(pNode != NULL)
			strValue = pNode->Value();
	}
	else
	{
		TiXmlNode* pNode = m_pCurNode->FirstChildElement();
		for( ; pNode != NULL; pNode = pNode->NextSiblingElement())
		{
			if(strcasecmp(pNode->Value(), pszKey) != 0)
				continue;

			TiXmlNode* pTmp = pNode->FirstChild();
			if(pTmp != NULL)
				strValue = pTmp->Value();
			break;
		}
	}

	return !strValue.empty();
}

bool CCfgFromXml::GetTextValue(const char* pszKey, unsigned short& usValue)
{
	std::string strValue;
	if(false == GetTextValue(pszKey, strValue))
		return false;

	usValue = atoi(strValue.c_str());
	return true;
}

bool CCfgFromXml::GetTextValue(const char* pszKey, int& intValue)
{
	std::string strValue;
	if(false == GetTextValue(pszKey, strValue))
		return false;

	intValue = atoi(strValue.c_str());
	return true;
}

bool CCfgFromXml::GetTextValue(const char* pszKey, unsigned int& uintValue)
{
	std::string strValue;
	if(false == GetTextValue(pszKey, strValue))
		return false;

	uintValue = atol(strValue.c_str());
	return true;
}
