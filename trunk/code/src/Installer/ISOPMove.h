#pragma once
#include "isopbase.h"

class CISOPMove :
	public CISOPBase
{
public:
	CISOPMove(void);
	~CISOPMove(void);
public:
	const std::wstring toString() const;
	OpErrorCode operator()() const;
	LONG GetValue() const { return 1; };
	ISOPType GetType() const { return ISOP_Type_Move; };
	OpErrorCode SetLine(const std::wstring &wstrLine);
};
