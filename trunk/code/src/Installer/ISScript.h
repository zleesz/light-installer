#pragma once
#include "GlobalDef.h"
#include "ScriptParser.h"
#include "ISSection.h"
#include <map>
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
	
	std::map<std::wstring, CISSection*> m_mapSection;
	CISSection* m_pSection;
private:
	void ClearSection();
public:
	BOOL Load(const TCHAR* tszPath);

public:
	// IScriptParserEvent
	void OnParserError(SPErrorCode errorCode, const std::wstring& wstrPath);
	void OnParserSucc(const std::wstring& wstrPath);
	void OnParserSection(const std::wstring& wstrSection, const std::wstring& wstrPath);
	void OnParserOpLine(const std::wstring& wstrOpLine, const std::wstring& wstrPath);
};
