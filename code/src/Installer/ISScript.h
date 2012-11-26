#pragma once
#include "GlobalDef.h"
#include "ScriptParser.h"
#include <string>
#include <assert.h>

class CISScript
	: public IScriptParserEvent
{
public:
	CISScript(void);
	~CISScript(void);
private:
	std::wstring m_wstrPath;
	CScriptParser* m_Parser;
public:
	BOOL Load(const TCHAR* tszPath);

public:
	// IScriptParserEvent
	void OnParserError(SPErrorCode errorCode, const std::wstring& wstrPath);
	void OnParserSucc(const std::wstring& wstrPath);
	void OnParserLine(const std::string& strLine, const std::wstring& wstrPath);
};
