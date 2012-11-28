#include "StdAfx.h"
#include "ISScript.h"

CISScript::CISScript(void)
{
	m_Parser = GET_SINGLETON(CScriptParser);
	m_pSection = NULL;
}

CISScript::~CISScript(void)
{
	m_Parser = NULL;
	m_pSection = NULL;
	ClearSection();
}

void CISScript::ClearSection()
{
	std::map<std::wstring, CISSection*>::iterator it = m_mapSection.begin();
	for(; it != m_mapSection.end(); it++)
	{
		delete it->second;
	}
	m_mapSection.clear();
}

BOOL CISScript::Load(const TCHAR* tszPath)
{
	LOGENTER();
	SPErrorCode errorCode = m_Parser->ParserScript(tszPath, static_cast<IScriptParserEvent*>(this));
	LOGDEBUG(_T("errorCode=%08X"), errorCode);
	BOOL bRet = (errorCode == SP_ERROR_NO_ERROR);
	if(bRet)
	{
		// 解析成功
	}
	else
	{
		assert(false && "脚本解析出错！");
	}
	return bRet;
}

void CISScript::OnParserError(SPErrorCode errorCode, const std::wstring& wstrPath)
{
	LOGENTER();
	m_pSection = NULL;
	ClearSection();
}

void CISScript::OnParserSucc(const std::wstring& wstrPath)
{
	LOGENTER();
	m_pSection = NULL;
}

void CISScript::OnParserSection(const std::wstring& wstrSection, const std::wstring& wstrPath)
{
	LOGENTER();
	CISSection* pSection = new CISSection();
	pSection->SetName(wstrSection);
	m_mapSection.insert(std::make_pair(wstrSection, pSection));
	m_pSection = pSection;
}

void CISScript::OnParserOpLine(const std::wstring& wstrOpLine, const std::wstring& wstrPath)
{
	LOGENTER();
	assert(NULL != m_pSection);
	if(m_pSection)
	{
		m_pSection->AddOperation(wstrOpLine);
	}
}