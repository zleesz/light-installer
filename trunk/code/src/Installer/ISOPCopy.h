#pragma once
#include "isopbase.h"

class CISOPCopy :
	public CISOPBase
{
public:
	CISOPCopy(void);
	~CISOPCopy(void);
private:
	std::wstring m_wstrSrc;
	std::wstring m_wstrDest;
public:
	const std::wstring toString() const;
	OpErrorCode operator()() const;
	LONG GetValue() const { return 1; };
	ISOPType GetType() const { return ISOP_Type_Copy; };
	OpErrorCode SetLine(const std::wstring &wstrLine);
};
