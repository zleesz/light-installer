#pragma once
#include "GlobalDef.h"
#include "ISOPBase.h"
#include <assert.h>

class CISOPClassFactory
{
private:
	CISOPClassFactory();
public:
	virtual ~CISOPClassFactory(void);
	SINGLETON_CLASS(CISOPClassFactory);
public:
	CISOPBase* CreateISOP(const std::wstring& wstrType);
	CISOPBase* CreateISOP(const ISOPType& isopType);
};
