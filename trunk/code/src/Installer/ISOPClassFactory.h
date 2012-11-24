#pragma once
#include "GlobalDef.h"
#include "ISOPBase.h"

class CISOPClassFactory
{
public:
	virtual ~CISOPClassFactory(void);
	SINGLETON_CLASS(CISOPClassFactory);
public:
	CISOPBase* CreateISOP(const std::wstring& wstrLine);
	CISOPBase* CreateISOP(const ISOPType& isopType);
};
