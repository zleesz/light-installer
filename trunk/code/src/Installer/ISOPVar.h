#pragma once
#include "isopbase.h"

class CISOPVar :
	public CISOPBase
{
public:
	CISOPVar(void);
	~CISOPVar(void);
public:
	const std::wstring toString() const;
	OpErrorCode operator()() const;
	LONG GetValue() const { return 0; };
	ISOPType GetType() const { return ISOP_Type_Var; };
	OpErrorCode SetLine(const std::wstring &wstrLine);
};
