#pragma once
#include "GlobalDef.h"
#include "ScriptParser.h"
#include <string>
#include <assert.h>

class CISScript
{
public:
	CISScript(void);
	~CISScript(void);
private:
	std::wstring m_wstrPath;
	CScriptParser* m_Parser;
public:
	BOOL Load(const TCHAR* tszPath);
};
