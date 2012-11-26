#pragma once
#include "isopbase.h"

class CISOPRmdir :
	public CISOPBase
{
public:
	CISOPRmdir(void);
	~CISOPRmdir(void);
public:
	const std::wstring toString() const;
	OpErrorCode operator()() const;
	LONG GetValue() const { return 2; };
	ISOPType GetType() const { return ISOP_Type_Rmdir; };
	OpErrorCode SetLine(const std::string &strLine);
};
