#pragma once
#include "GlobalDef.h"
#include "ISOPLog.h"

class IScriptParserEvent
{
public:
	virtual void OnParserError(SPErrorCode errorCode, const std::wstring& wstrPath) = 0;
	virtual void OnParserSucc(const std::wstring& wstrPath) = 0;
	virtual void OnParserSection(const std::wstring& wstrSection, const std::wstring& wstrPath) = 0;
	virtual void OnParserOpLine(const std::wstring& wstrOpLine, const std::wstring& wstrPath) = 0;
};

class CScriptParser
{
public:
	virtual ~CScriptParser(void);
	SINGLETON_CLASS2(CScriptParser);
private:
	typedef struct _tagScanLine
	{
		LONG lnLine;
		std::wstring wstrLine;
		_tagScanLine() : lnLine(-1) {}
	} ScanLine;
	ScanLine m_ScanLine;
	void Init();
public:
	inline LONG GetErrorLine() const
	{
		return m_ScanLine.lnLine;
	}
	inline const std::wstring& GetErrorLineStr() const
	{
		return m_ScanLine.wstrLine;
	}
public:
	SPErrorCode ParserScript(const std::wstring& wstrPath, IScriptParserEvent* pIEvent);
};
