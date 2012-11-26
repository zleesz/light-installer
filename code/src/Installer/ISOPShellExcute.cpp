#include "StdAfx.h"
#include "ISOPShellExcute.h"

CISOPShellExcute::CISOPShellExcute(void)
{
}

CISOPShellExcute::~CISOPShellExcute(void)
{
}

const std::wstring CISOPShellExcute::toString() const
{
	std::wstring wstrDesc;
	return wstrDesc;
}

OpErrorCode CISOPShellExcute::operator()() const
{
	OpErrorCode errorCode = OP_ERROR_NO_ERROR;
	return errorCode;
}

OpErrorCode CISOPShellExcute::SetLine(const std::string &strLine)
{
	OpErrorCode errorCode = OP_ERROR_NO_ERROR;
	return errorCode;
}