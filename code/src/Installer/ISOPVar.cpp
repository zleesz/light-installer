#include "StdAfx.h"
#include "ISOPVar.h"

CISOPVar::CISOPVar(void)
{
}

CISOPVar::~CISOPVar(void)
{
}

const std::wstring CISOPVar::toString() const
{
	std::wstring wstrDesc;
	return wstrDesc;
}

OpErrorCode CISOPVar::operator()() const
{
	OpErrorCode errorCode = OP_ERROR_NO_ERROR;
	return errorCode;
}

OpErrorCode CISOPVar::SetLine(const std::string &strLine)
{
	OpErrorCode errorCode = OP_ERROR_NO_ERROR;
	return errorCode;
}