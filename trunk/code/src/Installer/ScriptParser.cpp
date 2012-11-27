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
	if(!PathFileExists(wstrPath.c_str()))
	{
		errorCode = SP_ERROR_FILE_NOT_FOUND;
		if(pIEvent)
			pIEvent->OnParserError(errorCode, wstrPath);
		return errorCode;
	}
	// 读文件，解析脚本
	std::ifstream f;
	f.open(wstrPath.c_str(), std::ios::_Nocreate);
	if(f.fail())
	{
		errorCode = SP_ERROR_FILE_OPEN_FAILED;
		if(pIEvent)
			pIEvent->OnParserError(errorCode, wstrPath);
		return errorCode;
	}
	std::string s;
	while(getline(f, s))
	{
		if(s[0] == '[')
		{
			// 应该放到这里处理解析出错的情况。
			// script里面处理每一行操作解析出错情况。
			std::string::size_type stLeft = s.find_first_of(']');
			if(stLeft == std::string::npos)
			{
				// 脚本错误，没找到 ']'
				// pIEvent
				break;
			}
			else
			{
				// 找到了，再找一下 注释 "//" 标记有没有。
				// 如果没有，就解析成功
				// 如果有的话，并且在]前，出错。
				// 如果在]后，判断下之间除了空格还有没有其他字符。
				// 有其他字符，脚本出错，没有就回调 OnParserSection
			}
		}
		else if(pIEvent)
		{
			pIEvent->OnParserOpLine(s, wstrPath);
		}
	}
	pIEvent->OnParserSucc(wstrPath);
	return errorCode;
}
