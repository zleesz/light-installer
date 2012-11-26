#pragma once
#include "isopbase.h"

class CISOPRegister :
	public CISOPBase
{
public:
	CISOPRegister(void);
	~CISOPRegister(void);
public:
	const std::wstring toString() const;
	OpErrorCode operator()() const;
	LONG GetValue() const { return 2; };
	ISOPType GetType() const { return ISOP_Type_Reg; };
	OpErrorCode SetLine(const std::string &strLine);
};
