#pragma once
#include "GlobalDef.h"

class IScriptParserEvent
{
public:
	virtual void OnParserError(SPErrorCode errorCode, const std::wstring& wstrPath) = 0;
	virtual void OnParserSucc(const std::wstring& wstrPath) = 0;
	virtual void OnParserLine(const std::string& strLine, const std::wstring& wstrPath) = 0;
};

class CScriptParser
{
public:
	virtual ~CScriptParser(void);
	SINGLETON_CLASS2(CScriptParser);
public:
	SPErrorCode ParserScript(const std::wstring& wstrPath, IScriptParserEvent* pIEvent);
};
