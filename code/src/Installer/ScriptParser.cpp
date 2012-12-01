#include "StdAfx.h"
#include "ScriptParser.h"
#include <fstream>
#include "Utility.h"

CScriptParser::~CScriptParser(void)
{
}

void CScriptParser::Init()
{
	m_ScanLine.lnLine = -1;
	m_ScanLine.wstrLine = L"";
}

SPErrorCode CScriptParser::ParserScript(const std::wstring& wstrPath, IScriptParserEvent* pIEvent)
{
	LOGENTER();
	Init();
	SPErrorCode errorCode = SP_ERROR_NO_ERROR;
	if(!PathFileExists(wstrPath.c_str()))
	{
		errorCode = SP_ERROR_FILE_NOT_FOUND;
		if(pIEvent)
			pIEvent->OnParserError(errorCode, wstrPath);
		return errorCode;
	}
	// ���ļ��������ű�
	// һ���� UTF-8 ����
	std::ifstream f;
	f.open(wstrPath.c_str(), std::ios::_Nocreate || std::ios::binary);
	if(f.fail())
	{
		errorCode = SP_ERROR_FILE_OPEN_FAILED;
		if(pIEvent)
			pIEvent->OnParserError(errorCode, wstrPath);
		return errorCode;
	}
	m_ScanLine.lnLine = 0;
	std::string s;
	while(getline(f, s))
	{
		if(0 == m_ScanLine.lnLine && s.length() >= 3)
		{
			// �ж�UTF-8����BOM������BOM
			// ǰ�����ֽ�0xEF��0xBB��0xBF
			if(s[0] == (char)0xEF && s[1] == (char)0xBB && s[2] == (char)0xBF)
			{
				// ��BOM�ļ�ͷ��ȥ��
				s = &s[3];
			}
		}
		std::wstring wstr;
		Utility::MBCSToUnicode(s.c_str(), wstr);
		Utility::TrimLeft(wstr);
		m_ScanLine.wstrLine = wstr;
		m_ScanLine.lnLine++;
		if(wstr.length() <= 0)
		{
			continue;
		}
		else if(wstr[0] == L'/' && wstr[1] == L'/')
		{
			// ע��
			// TODO: /* */ ����ע��
			continue;
		}
		else if(wstr[0] == L'[')
		{
			// Ӧ�÷ŵ����ﴦ���������������
			// script���洦��ÿһ�в����������������
			std::wstring::size_type stLeft = wstr.find_first_of(L']');
			if(stLeft == std::wstring::npos)
			{
				// �ű�����û�ҵ� ']'
				// pIEvent
				errorCode = SP_ERROR_SECTION_TOKEN_MISSING;
				if(pIEvent)
					pIEvent->OnParserError(errorCode, wstrPath);
				return errorCode;
			}
			else
			{
				// �ҵ��ˣ�����һ�� ע�� "//" �����û�С�
				// ���û�У��ͽ����ɹ�
				// ����еĻ���������]ǰ������
				// �����]���ж���֮����˿ո���û�������ַ���
				// �������ַ����ű�����û�оͻص� OnParserSection  
				std::wstring::size_type stComment = wstr.find_first_of(L"//");
				if(stComment == std::wstring::npos)
				{
					std::wstring wstrRight = &wstr[stLeft + 1];
					Utility::TrimLeft(wstrRight);
				}
				else if(stComment < stLeft)
				{
					errorCode = SP_ERROR_SECTION_TOKEN_MISSING;
					if(pIEvent)
						pIEvent->OnParserError(errorCode, wstrPath);
					return errorCode;
				}
				else
				{
					std::wstring wstrName = wstr.substr(1, stLeft - 1);
					if(pIEvent)
						pIEvent->OnParserSection(wstrName, wstrPath);
				}
			}
		}
		else if(pIEvent)
		{
			pIEvent->OnParserOpLine(wstr, wstrPath);
		}
	}
	pIEvent->OnParserSucc(wstrPath);
	return errorCode;
}
