#pragma once
#include "GlobalDef.h"
#include "ISOPClassFactory.h"

class CScriptParser
{
public:
	virtual ~CScriptParser(void);
	SINGLETON_CLASS(CScriptParser);
};
