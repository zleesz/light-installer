#pragma once
#include "GlobalDef.h"
#include "ISScript.h"
#include "ScriptParser.h"
#include <string>
#include <vector>
#include <map>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <assert.h>

class CISScriptManager
{
public:
	CISScriptManager(void);
	~CISScriptManager(void);
	SINGLETON_CLASS(CISScriptManager);
public:
	BOOL LoadScript(const TCHAR* pszPath);
private:
	BOOL _LoadScript(const TCHAR* pszPath);
private:
	std::vector<std::wstring> m_vecPath;
	std::map<std::wstring, CISScript*> m_mapScript;
};
