#include "StdAfx.h"
#include "ScriptParser.h"
#include <fstream>

CScriptParser::~CScriptParser(void)
{
}

SPErrorCode CScriptParser::ParserScript(const std::wstring& wstrPath, IScriptParserEvent* pIEvent)
{
	LOGENTER();
	SPErrorCode errorCode = SP_ERROR_NO_ERROR;
	// ���ļ��������ű�
	std::ifstream f;
	f.open(wstrPath.c_str(), std::ios::_Nocreate);
	if(f.fail())
	{
		if(pIEvent)
			pIEvent->OnParserError(SP_ERROR_FILE_OPEN_FAILED, wstrPath);
		return SP_ERROR_FILE_OPEN_FAILED;
	}
	std::string s;
	while(getline(f, s))
	{
		if(s[0] == '[')
		{
			// Ӧ�÷ŵ����ﴦ���������������
			// script���洦��ÿһ�в����������������
			std::string::size_type stLeft = s.find_first_of(']');
			if(stLeft == std::string::npos)
			{
				// �ű�����û�ҵ� ']'
				// pIEvent
				break;
			}
			else
			{
				// �ҵ��ˣ�����һ�� ע�� "//" �����û�С�
				// ���û�У��ͽ����ɹ�
				// ����еĻ���������]ǰ������
				// �����]���ж���֮����˿ո���û�������ַ���
				// �������ַ����ű�����û�оͻص� OnParserSection
			}
		}
		else if(pIEvent)
		{
			pIEvent->OnParserOpLine(s, wstrPath);
		}
	}
	pIEvent->OnParserSucc(wstrPath);
	return errorCode;
}
