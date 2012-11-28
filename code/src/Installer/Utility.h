#pragma once

#include <string>

namespace Utility
{
	static bool MBCSToUnicode(const char* src, std::wstring& wstr)
	{
		wstr = L"";
		if(!src)
			return false;
		size_t iLen = strlen(src);
		if(iLen > 0)
		{
			wchar_t* szm = new wchar_t[iLen * 4];
			ZeroMemory(szm, iLen * 4);
			int nLen = MultiByteToWideChar(CP_UTF8, 0, src,(int)iLen, szm, (int)iLen*4); 
			szm [nLen] = '\0';
			wstr = szm;
			delete [] szm;
			return true;
		}
		return false;
	}

	static inline void TrimLeft(std::string& strSrc, char* pchs = " \t")
	{
		if(pchs == NULL)
		{
			return;
		}
		char* psz = &strSrc[0];
		for(int i = 0; i < (int)strSrc.length(); i++)
		{
			bool bIsTrim = false;
			for(int j = 0; j < (int)strlen(pchs); j++)
			{
				if(strSrc[i] == pchs[j])
				{
					psz = &strSrc[i + 1];
					bIsTrim = true;
				}
			}
			if(!bIsTrim)
				break;
		}
		strSrc = psz;
	}
	static inline void TrimLeft(std::wstring& wstrSrc, wchar_t* pwchs = L" \t")
	{
		if(pwchs == NULL)
		{
			return;
		}
		wchar_t* psz = &wstrSrc[0];
		for(int i = 0; i < (int)wstrSrc.length(); i++)
		{
			bool bIsTrim = false;
			for(int j = 0; j < (int)wcslen(pwchs); j++)
			{
				if(wstrSrc[i] == pwchs[j])
				{
					psz = &wstrSrc[i + 1];
					bIsTrim = true;
				}
			}
			if(!bIsTrim)
				break;
		}
		wstrSrc = psz;
	}
}
