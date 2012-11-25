#pragma once
#include "GlobalDef.h"

class CScriptParser
{
public:
	virtual ~CScriptParser(void);
	SINGLETON_CLASS2(CScriptParser);
public:
	SPErrorCode ParserScript(const std::wstring& wtrPath);
};
