#ifndef __CFGFROMXML_H__
#define __CFGFROMXML_H__

#include <string>

class CCfgFromXml
{
public:
	CCfgFromXml(void);
	~CCfgFromXml(void);

public:
	bool LoadXml(const char* pszXmlFile);
	bool LoadPwdXml(const char* pszXmlFile);

	bool MoveRootElement();
	bool MoveToElement(const char* pszKey);

	bool GetAttrValue(const char* pszKey, const char* pszAttr, std::string& strValue);

	bool GetTextValue(const char* pszKey, std::string& strValue);
	bool GetTextValue(const char* pszKey, unsigned short& usValue);
	bool GetTextValue(const char* pszKey, int& intValue);
	bool GetTextValue(const char* pszKey, unsigned int& uintValue);

private:
	class TiXmlDocument* m_pDocument;
	class TiXmlElement* m_pCurElement;
	class TiXmlNode* m_pCurNode;

};

#endif
