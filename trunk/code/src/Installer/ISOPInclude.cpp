#include "StdAfx.h"
#include "ISOPInclude.h"

CISOPInclude::CISOPInclude(void)
{
}

CISOPInclude::~CISOPInclude(void)
{
}

const std::wstring CISOPInclude::toString() const
{
	std::wstring wstrDesc;
	return wstrDesc;
}

OpErrorCode CISOPInclude::operator()() const
{
	OpErrorCode errorCode = OP_ERROR_NO_ERROR;
	return errorCode;
}

OpErrorCode CISOPInclude::SetLine(const std::wstring &wstrLine)
{
	OpErrorCode errorCode = OP_ERROR_NO_ERROR;
	return errorCode;
}