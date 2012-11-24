#pragma once
// 支持C++风格的打日志类
#ifndef ISOPLOG
#define LOGENTER

#else
#define LOGENTER CISOPLog
#define LOGDEBUG  __if_exists(this) {if(!CTSLog::IsModuleUnloading(__TFUNCTION__)) CTSLog::GetInstance(__TFUNCTION__ ,(LPVOID)this).TSDebug} __if_not_exists(this){if(!CTSLog::IsModuleUnloading(__TFUNCTION__))  CTSLog::GetInstance(__TFUNCTION__).TSDebug}

class CISOPLog
{
public:
	~CISOPLog()
	{
		
	}
private:
	CISOPLog()
	{
		//
	}
public:
	static CISOPLog* pThis;
	static CISOPLog* GetInstance()
	{
		static CISOPLog s_isopInstance;
		return &s_isopInstance;
	}
private:
	
};

__declspec(selectany) CISOPLog* CISOPLog::pThis = CISOPLog::GetInstance();
#endif