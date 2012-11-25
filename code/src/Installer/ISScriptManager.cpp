#include "StdAfx.h"
#include "ISScriptManager.h"

CISScriptManager::CISScriptManager(void)
{
}

CISScriptManager::~CISScriptManager(void)
{
}

BOOL CISScriptManager::_LoadScript(const TCHAR* pszPath)
{
	LOGENTER();
	BOOL bRet = TRUE;
	CISScript* Script = new CISScript();
	Script->Load(pszPath);
	return bRet;
}

BOOL CISScriptManager::LoadScript(const TCHAR* pszPath)
{
	LOGENTER();
	BOOL bRet = TRUE;
	if(PathIsRelative(pszPath))
	{
		TCHAR tszPath[MAX_PATH] = {0};
		GetModuleFileName(NULL, tszPath, MAX_PATH);
		PathAppend(tszPath, pszPath);
		if(PathFileExists(tszPath))
		{
			bRet = _LoadScript(tszPath);
		}
		else
		{
			assert(false && "脚本路径不存在！");
			bRet = FALSE;
		}
	}
	else if(PathFileExists(pszPath))
	{
		bRet = _LoadScript(pszPath);
	}
	else
	{
		assert(false && "脚本路径不存在！");
		bRet = FALSE;
	}
	return bRet;
}
