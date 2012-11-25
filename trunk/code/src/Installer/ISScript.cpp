#include "StdAfx.h"
#include "ISScript.h"

CISScript::CISScript(void)
{
	m_Parser = CScriptParser::GetInstance();
}

CISScript::~CISScript(void)
{
	m_Parser = NULL;
}

BOOL CISScript::Load(const TCHAR* tszPath)
{
	LOGENTER();
	SPErrorCode errorCode = m_Parser->ParserScript(tszPath);
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
