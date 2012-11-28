#include "StdAfx.h"
#include "ISOPMove.h"

CISOPMove::CISOPMove(void)
{
}

CISOPMove::~CISOPMove(void)
{
}

const std::wstring CISOPMove::toString() const
{
	std::wstring wstrDesc;
	return wstrDesc;
}

OpErrorCode CISOPMove::operator()() const
{
	OpErrorCode errorCode = OP_ERROR_NO_ERROR;
	return errorCode;
}

OpErrorCode CISOPMove::SetLine(const std::wstring &wstrLine)
{
	OpErrorCode errorCode = OP_ERROR_NO_ERROR;
	return errorCode;
}