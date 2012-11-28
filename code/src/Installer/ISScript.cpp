#include "StdAfx.h"
#include "ISScript.h"

CISScript::CISScript(void)
{
	m_Parser = GET_SINGLETON(CScriptParser);
}

CISScript::~CISScript(void)
{
	m_Parser = NULL;
}

BOOL CISScript::Load(const TCHAR* tszPath)
{
	LOGENTER();
	SPErrorCode errorCode = m_Parser->ParserScript(tszPath, static_cast<IScriptParserEvent*>(this));
	LOGDEBUG(_T("errorCode=%08X"), errorCode);
	BOOL bRet = (errorCode == SP_ERROR_NO_ERROR);
	if(bRet)
	{
		// �����ɹ�
	}
	else
	{
		assert(false && "�ű���������");
	}
	return bRet;
}

void CISScript::OnParserError(SPErrorCode errorCode, const std::wstring& wstrPath)
{
	LOGENTER();
}

void CISScript::OnParserSucc(const std::wstring& wstrPath)
{

}

void CISScript::OnParserSection(const std::wstring& wstrSection, const std::wstring& wstrPath)
{

}

void CISScript::OnParserOpLine(const std::wstring& wstrOpLine, const std::wstring& wstrPath)
{

}