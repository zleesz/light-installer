#include "StdAfx.h"
#include "ISOPDel.h"

CISOPDel::CISOPDel(void)
{
}

CISOPDel::~CISOPDel(void)
{
}

const std::wstring CISOPDel::toString() const
{
	std::wstring wstrDesc;
	return wstrDesc;
}

OpErrorCode CISOPDel::operator()() const
{
	OpErrorCode errorCode = OP_ERROR_NO_ERROR;
	return errorCode;
}

OpErrorCode CISOPDel::SetLine(const std::wstring &wstrLine)
{
	OpErrorCode errorCode = OP_ERROR_NO_ERROR;
	return errorCode;
}