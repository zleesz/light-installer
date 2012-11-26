#include "StdAfx.h"
#include "ScriptParser.h"

CScriptParser::~CScriptParser(void)
{
}

SPErrorCode CScriptParser::ParserScript(const std::wstring& wtrPath, const IScriptParserEvent* pIEvent)
{
	LOGENTER();
	SPErrorCode errorCode = SP_ERROR_NO_ERROR;
	// 读文件，解析脚本
	return errorCode;
}
