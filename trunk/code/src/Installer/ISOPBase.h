#pragma once
#include <string>
typedef enum _enumOpErrorCode
{
	
}OpErrorCode;

class CISOPBase
{
public:
	CISOPBase(void);
	virtual ~CISOPBase(void);
public:
	virtual const std::wstring &toString() const = 0;
	virtual OpErrorCode operator()() const = 0;
};
