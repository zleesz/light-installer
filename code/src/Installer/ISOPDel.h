#pragma once
#include "isopbase.h"

class CISOPDel :
	public CISOPBase
{
public:
	CISOPDel(void);
	~CISOPDel(void);
public:
	const std::wstring toString() const;
	OpErrorCode operator()() const;
	LONG GetValue() const { return 1; };
	ISOPType GetType() const { return ISOP_Type_Del; };
	OpErrorCode SetLine(const std::string &strLine);
};
