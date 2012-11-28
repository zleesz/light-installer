#pragma once
#include "GlobalDef.h"
#include "ISOPBase.h"
#include "ISOPClassFactory.h"
#include <string>
#include <vector>
#include <assert.h>

class CISSection
{
public:
	CISSection(void);
	~CISSection(void);
private:
	std::wstring m_wstrName;
	std::vector<CISOPBase*> m_vecOP;
	CISOPClassFactory* m_pClassFactory;
public:
	void SetName(const std::wstring& wstrName);
	const std::wstring& GetName() const;
	void AddOperation(const std::wstring& wstrLine);
};
