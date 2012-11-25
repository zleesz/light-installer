#include "StdAfx.h"
#include "ISOPPath.h"

CISOPPath::CISOPPath(void)
{
}

CISOPPath::~CISOPPath(void)
{
}

const std::wstring &CISOPPath::toString() const
{
	std::wstring wstrDesc;
	return wstrDesc;
}

OpErrorCode CISOPPath::operator()() const
{
	OpErrorCode errorCode = OP_ERROR_NO_ERROR;
	return errorCode;
}

OpErrorCode CISOPPath::SetLine(const std::wstring &wstrLine)
{
	OpErrorCode errorCode = OP_ERROR_NO_ERROR;
	return errorCode;
}