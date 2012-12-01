#include "StdAfx.h"
#include "ScriptParser.h"
#include <fstream>
#include "Utility.h"

CScriptParser::~CScriptParser(void)
{
}

void CScriptParser::Init()
{
	m_ScanLine.lnLine = -1;
	m_ScanLine.wstrLine = L"";
}

SPErrorCode CScriptParser::ParserScript(const std::wstring& wstrPath, IScriptParserEvent* pIEvent)
{
	LOGENTER();
	Init();
	SPErrorCode errorCode = SP_ERROR_NO_ERROR;
	if(!PathFileExists(wstrPath.c_str()))
	{
		errorCode = SP_ERROR_FILE_NOT_FOUND;
		if(pIEvent)
			pIEvent->OnParserError(errorCode, wstrPath);
		return errorCode;
	}
	// 读文件，解析脚本
	// 一定是 UTF-8 编码
	std::ifstream f;
	f.open(wstrPath.c_str(), std::ios::_Nocreate || std::ios::binary);
	if(f.fail())
	{
		errorCode = SP_ERROR_FILE_OPEN_FAILED;
		if(pIEvent)
			pIEvent->OnParserError(errorCode, wstrPath);
		return errorCode;
	}
	m_ScanLine.lnLine = 0;
	std::string s;
	while(getline(f, s))
	{
		if(0 == m_ScanLine.lnLine && s.length() >= 3)
		{
			// 判断UTF-8是有BOM还是无BOM
			// 前两个字节0xEF、0xBB、0xBF
			if(s[0] == (char)0xEF && s[1] == (char)0xBB && s[2] == (char)0xBF)
			{
				// 有BOM文件头，去掉
				s = &s[3];
			}
		}
		std::wstring wstr;
		Utility::MBCSToUnicode(s.c_str(), wstr);
		Utility::TrimLeft(wstr);
		m_ScanLine.wstrLine = wstr;
		m_ScanLine.lnLine++;
		if(wstr.length() <= 0)
		{
			continue;
		}
		else if(wstr[0] == L'/' && wstr[1] == L'/')
		{
			// 注释
			// TODO: /* */ 多行注释
			continue;
		}
		else if(wstr[0] == L'[')
		{
			// 应该放到这里处理解析出错的情况。
			// script里面处理每一行操作解析出错情况。
			std::wstring::size_type stLeft = wstr.find_first_of(L']');
			if(stLeft == std::wstring::npos)
			{
				// 脚本错误，没找到 ']'
				// pIEvent
				errorCode = SP_ERROR_SECTION_TOKEN_MISSING;
				if(pIEvent)
					pIEvent->OnParserError(errorCode, wstrPath);
				return errorCode;
			}
			else
			{
				// 找到了，再找一下 注释 "//" 标记有没有。
				// 如果没有，就解析成功
				// 如果有的话，并且在]前，出错。
				// 如果在]后，判断下之间除了空格还有没有其他字符。
				// 有其他字符，脚本出错，没有就回调 OnParserSection  
				std::wstring::size_type stComment = wstr.find_first_of(L"//");
				if(stComment == std::wstring::npos)
				{
					std::wstring wstrRight = &wstr[stLeft + 1];
					Utility::TrimLeft(wstrRight);
				}
				else if(stComment < stLeft)
				{
					errorCode = SP_ERROR_SECTION_TOKEN_MISSING;
					if(pIEvent)
						pIEvent->OnParserError(errorCode, wstrPath);
					return errorCode;
				}
				else
				{
					std::wstring wstrName = wstr.substr(1, stLeft - 1);
					if(pIEvent)
						pIEvent->OnParserSection(wstrName, wstrPath);
				}
			}
		}
		else if(pIEvent)
		{
			pIEvent->OnParserOpLine(wstr, wstrPath);
		}
	}
	pIEvent->OnParserSucc(wstrPath);
	return errorCode;
}
