#include "StdAfx.h"
#include "ISOPClassFactory.h"
#include "ISOPCopy.h"
#include "ISOPDel.h"
#include "ISOPInclude.h"
#include "ISOPMove.h"
#include "ISOPPath.h"
#include "ISOPRegister.h"
#include "ISOPRmdir.h"
#include "ISOPShellExcute.h"
#include "ISOPVar.h"

CISOPClassFactory::CISOPClassFactory()
{
	LOGDEBUG(_T("CISOPClassFactory constructor."));
}

CISOPClassFactory::~CISOPClassFactory(void)
{
}

CISOPBase* CISOPClassFactory::CreateISOP(const std::wstring& wstrType)
{
	LOGENTER();
	CISOPBase* pISOPBase = NULL;
	if(0 == _wcsicmp(wstrType.c_str(), L"copy"))
	{
		pISOPBase = new CISOPCopy();
	}
	else if(0 == _wcsicmp(wstrType.c_str(), L"del") || 0 == _wcsicmp(wstrType.c_str(), L"delete"))
	{
		pISOPBase = new CISOPDel();
	}
	else if(0 == _wcsicmp(wstrType.c_str(), L"include"))
	{
		pISOPBase = new CISOPInclude();
	}
	else if(0 == _wcsicmp(wstrType.c_str(), L"move"))
	{
		pISOPBase = new CISOPMove();
	}
	else if(0 == _wcsicmp(wstrType.c_str(), L"path"))
	{
		pISOPBase = new CISOPPath();
	}
	else if(0 == _wcsicmp(wstrType.c_str(), L"reg") || 0 == _wcsicmp(wstrType.c_str(), L"register"))
	{
		pISOPBase = new CISOPRegister();
	}
	else if(0 == _wcsicmp(wstrType.c_str(), L"rmdir"))
	{
		pISOPBase = new CISOPRmdir();
	}
	else if(0 == _wcsicmp(wstrType.c_str(), L"shext") || 0 == _wcsicmp(wstrType.c_str(), L"shellexcute"))
	{
		pISOPBase = new CISOPShellExcute();
	}
	else if(0 == _wcsicmp(wstrType.c_str(), L"var"))
	{
		pISOPBase = new CISOPVar();
	}
	else
	{
		assert(false && "不支持的操作类型！");
	}
	return pISOPBase;
}

CISOPBase* CISOPClassFactory::CreateISOP(const ISOPType& isopType)
{
	LOGENTER();
	CISOPBase* pISOPBase = NULL;
	switch(isopType)
	{
	case ISOP_Type_Copy:
		pISOPBase = new CISOPCopy();
		break;
	case ISOP_Type_Del:
		pISOPBase = new CISOPDel();
		break;
	case ISOP_Type_Include:
		pISOPBase = new CISOPInclude();
		break;
	case ISOP_Type_Move:
		pISOPBase = new CISOPMove();
		break;
	case ISOP_Type_Path:
		pISOPBase = new CISOPPath();
		break;
	case ISOP_Type_Reg:
		pISOPBase = new CISOPRegister();
		break;
	case ISOP_Type_Rmdir:
		pISOPBase = new CISOPRmdir();
		break;
	case ISOP_Type_SE:
		pISOPBase = new CISOPShellExcute();
		break;
	case ISOP_Type_Var:
		pISOPBase = new CISOPVar();
		break;
	default:
		assert(false && "不支持的操作类型！");
		break;
	}
	return pISOPBase;
}