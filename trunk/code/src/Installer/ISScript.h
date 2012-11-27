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
	void OnParserSection(const std::string& strSection, const std::wstring& wstrPath);
	void OnParserOpLine(const std::string& strOpLine, const std::wstring& wstrPath);
};
