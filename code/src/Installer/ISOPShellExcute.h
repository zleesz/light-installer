#pragma once
#include "isopbase.h"

class CISOPShellExcute :
	public CISOPBase
{
public:
	CISOPShellExcute(void);
	~CISOPShellExcute(void);
public:
	const std::wstring &toString() const;
	OpErrorCode operator()() const;
	LONG GetValue() const { return 1; };
	ISOPType GetType() const { return ISOP_Type_SE; };
	OpErrorCode SetLine(const std::wstring &wstrLine);
};
