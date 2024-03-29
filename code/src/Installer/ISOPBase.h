#pragma once
#include <string>
#include "GlobalDef.h"

typedef enum _enumISOPType
{
	ISOP_Type_Copy = 1,
	ISOP_Type_Del,
	ISOP_Type_Include,
	ISOP_Type_Move,
	ISOP_Type_Path,
	ISOP_Type_Reg,
	ISOP_Type_Rmdir,
	ISOP_Type_SE,
	ISOP_Type_Var,
}ISOPType;

class IOPStatusChangeEvent
{
public:
	virtual void OnStatusChange(const std::wstring& wstrStatus) = 0;
};

class CISOPBase
{
private:
	IOPStatusChangeEvent* m_pIEvent;
public:
	CISOPBase() : m_pIEvent(NULL) {}
public:
	virtual const std::wstring toString() const = 0;
	void Bind(IOPStatusChangeEvent* pIEvent);
	virtual OpErrorCode operator()() const = 0;
	virtual LONG GetValue() const = 0;
	virtual ISOPType GetType() const = 0;
	virtual OpErrorCode SetLine(const std::wstring &wstrLine) = 0;
};
