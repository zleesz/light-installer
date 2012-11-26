#include "StdAfx.h"
#include "ScriptParser.h"
#include <fstream>

CScriptParser::~CScriptParser(void)
{
}

SPErrorCode CScriptParser::ParserScript(const std::wstring& wstrPath, IScriptParserEvent* pIEvent)
{
	LOGENTER();
	SPErrorCode errorCode = SP_ERROR_NO_ERROR;
	// 读文件，解析脚本
	std::ifstream f;
	f.open(wstrPath.c_str(), std::ios::_Nocreate);
	if(f.fail())
	{
		if(pIEvent)
			pIEvent->OnParserError(SP_ERROR_FILE_OPEN_FAILED, wstrPath);
		return SP_ERROR_FILE_OPEN_FAILED;
	}
	std::string s;
	while(getline(f, s))
	{
		if(pIEvent)
			pIEvent->OnParserLine(s, wstrPath);
	}
	pIEvent->OnParserSucc(wstrPath);
	return errorCode;
}
