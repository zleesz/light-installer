#pragma once
#include "isopbase.h"

class CISOPInclude :
	public CISOPBase
{
public:
	CISOPInclude(void);
	virtual ~CISOPInclude(void);
public:
	const std::wstring toString() const;
	OpErrorCode operator()() const;
	LONG GetValue() const { return 0; };
	ISOPType GetType() const { return ISOP_Type_Include; };
	OpErrorCode SetLine(const std::wstring &wstrLine);
};
