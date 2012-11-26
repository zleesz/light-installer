#include "StdAfx.h"
#include "ISSection.h"

CISSection::CISSection(void)
{
	m_pClassFactory = CISOPClassFactory::GetInstance();
}

CISSection::~CISSection(void)
{
	m_pClassFactory = NULL;
	std::vector<CISOPBase*>::iterator it = m_vecOP.begin();
	for(; it != m_vecOP.end(); it++)
	{
		delete *it;
	}
	m_vecOP.clear();
}

void CISSection::SetName(const std::wstring& wstrName)
{
	LOGENTER();
	if(m_wstrName.empty())
	{
		m_wstrName = wstrName;
	}
	else
	{
		assert(false && "section name 重复设置！");
	}
}

const std::wstring& CISSection::GetName() const
{
	return m_wstrName;
}

void CISSection::AddOperation(const std::wstring& wstrLine)
{
	LOGENTER();
	if(m_wstrName.empty())
	{
		assert(false && "AddOperation 之前要先调 SetName！");
		return;
	}
	CISOPBase* pISOPBase = m_pClassFactory->CreateISOP(m_wstrName);
	if(NULL != pISOPBase)
	{
		OpErrorCode errorCode = pISOPBase->SetLine(wstrLine);
		LOGDEBUG(_T("errorCode=%08X"), errorCode);
		m_vecOP.push_back(pISOPBase);
	}
}
