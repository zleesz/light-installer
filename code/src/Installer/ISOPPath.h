#pragma once
#include "isopbase.h"

class CISOPPath :
	public CISOPBase
{
public:
	CISOPPath(void);
	~CISOPPath(void);
public:
	const std::wstring toString() const;
	OpErrorCode operator()() const;
	LONG GetValue() const { return 0; };
	ISOPType GetType() const { return ISOP_Type_Path; };
	OpErrorCode SetLine(const std::string &strLine);
};
