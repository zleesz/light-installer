#include "StdAfx.h"
#include "ISOPCopy.h"

CISOPCopy::CISOPCopy(void)
{
}

CISOPCopy::~CISOPCopy(void)
{
}

const std::wstring CISOPCopy::toString() const
{
	std::wstring wstrDesc;
	wstrDesc += L"拷贝文件 从";
	wstrDesc += m_wstrSrc;
	wstrDesc += L"到";
	wstrDesc += m_wstrDest;
	return wstrDesc;
}

OpErrorCode CISOPCopy::operator()() const
{
	OpErrorCode errorCode = OP_ERROR_NO_ERROR;
	return errorCode;
}

OpErrorCode CISOPCopy::SetLine(const std::wstring &wstrLine)
{
	OpErrorCode errorCode = OP_ERROR_NO_ERROR;
	return errorCode;
}