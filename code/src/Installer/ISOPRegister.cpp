#include "StdAfx.h"
#include "ISOPRegister.h"

CISOPRegister::CISOPRegister(void)
{
}

CISOPRegister::~CISOPRegister(void)
{
}

const std::wstring CISOPRegister::toString() const
{
	std::wstring wstrDesc;
	return wstrDesc;
}

OpErrorCode CISOPRegister::operator()() const
{
	OpErrorCode errorCode = OP_ERROR_NO_ERROR;
	return errorCode;
}

OpErrorCode CISOPRegister::SetLine(const std::wstring &wstrLine)
{
	OpErrorCode errorCode = OP_ERROR_NO_ERROR;
	return errorCode;
}