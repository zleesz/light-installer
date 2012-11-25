#pragma once
// 支持C++风格的打日志类
#ifndef ISOPLOG
#define LOGENTER
#define LOGDEBUG
#else
#include <stdio.h>
#include <string>	//std::string strstr
#include <OAIdl.h>
#include <MLang.h> //codepage
#include <time.h>		//time
#pragma comment(lib, "Version.lib") //VerQueryValue

#if (defined(UNICODE) || defined(_UNICODE))
#define TFUNCTION2(x) L ## x
#define TFUNCTION(x) TFUNCTION2(x)
#define __TFUNCTION__  TFUNCTION(__FUNCTION__)
#define TFUNCSIG2(x) L ## x
#define TFUNCSIG(x) TFUNCSIG2(x)
#define __TFUNCSIG__  TFUNCSIG(__FUNCSIG__)
#define TTIMESTAMP2(x) L ## x
#define TTIMESTAMP(x) TTIMESTAMP2(x)
#define __TTIMESTAMP__  TTIMESTAMP(__TIMESTAMP__)
#define TTSLOG_CONFIG_DIR2(x) L ## x
#define TTSLOG_CONFIG_DIR(x) TTSLOG_CONFIG_DIR2(x)
#define __TTSLOG_CONFIG_DIR TTSLOG_CONFIG_DIR(TSLOG_CONFIG_DIR)
#define TTSLOG_GROUP2(x) L ## x
#define TTSLOG_GROUP(x) TTSLOG_GROUP2(x)
#define __TTSLOG_GROUP TTSLOG_GROUP(TSLOG_GROUP)
#define __TTSLOG_GROUP_DEFAULT TTSLOG_GROUP(TSLOG_GROUP_DEFAULT)
#else
#define __TFUNCTION__ __FUNCTION__
#define __TFUNCSIG__ __FUNCSIG__
#define __TTIMESTAMP__ __TIMESTAMP__
#define __TTSLOG_CONFIG_DIR TSLOG_CONFIG_DIR
#define __TTSLOG_GROUP TSLOG_GROUP
#define __TTSLOG_GROUP_DEFAULT TSLOG_GROUP_DEFAULT
#endif
typedef enum
{
	LEVEL_TRACE,
	LEVEL_DEBUG,
	LEVEL_INFO,
	LEVEL_WARN,
	LEVEL_ERROR,
	LEVEL_FATAL,
	LEVEL_PROMPT, //打印提示信息, 内部使用
	LEVEL_OFF
}LogLevel;

#define LOGDEBUG \
	__if_exists(this) {CISOPLog::GetInstance(__TFUNCTION__ ,(LPVOID)this)->TSDebug} \
	__if_not_exists(this){CISOPLog::GetInstance(__TFUNCTION__)->TSDebug}

#define LOGENTER() \
	__if_exists(this) { CISOPLog::CISOPLogEnter enter( __TFUNCSIG__ , ENTER_FUNCTION_FLAG __TFUNCTION__ ,__TFUNCSIG__, (LPCVOID)this); } \
	__if_not_exists(this){ CISOPLog::CISOPLogEnter enter(__TFUNCSIG__ ,ENTER_FUNCTION_FLAG __TFUNCTION__,__TFUNCSIG__, NULL); } 

#define MAX_USERDATA_SIZE			(4096 - 1)
#define MAX_PRIVATEDATA_SIZE		256
#define DEFAULT_MAX_LOGFILE_SIZE	20480
#define KBTYE						1024
#define DEFAULT_LOGFILE_PATH		"C:\\LOG\\"
#define OUTPUTLOG_FORLEVEL(XLevel, pszFormat)  va_list ap; va_start(ap, pszFormat);OutputLog(XLevel, pszFormat, ap);	va_end(ap);

class CISOPLog
{
public:
	~CISOPLog()
	{
		s_pThis->TSPrompt(_T("Module Unload. Path = %s"), s_szModuleFileName);
		TlsFree(CISOPLog::s_dwTlsIndex);
		TlsFree(CISOPLog::s_dwTlsIndex_this);
		if( INVALID_HANDLE_VALUE != s_pThis->s_hLogFile)
			::CloseHandle( s_pThis->s_hLogFile );
	}
private:
	CISOPLog()
	{
		//
		CISOPLog::s_dwTlsIndex = TlsAlloc();
		CISOPLog::s_dwTlsIndex_this = TlsAlloc();
		if(TLS_OUT_OF_INDEXES == CISOPLog::s_dwTlsIndex)
			CISOPLog::s_dwTlsIndex = TlsAlloc();
		if(TLS_OUT_OF_INDEXES == CISOPLog::s_dwTlsIndex_this)
			CISOPLog::s_dwTlsIndex_this = TlsAlloc();
		m_hLang = LoadLibrary(_T("mlang.dll"));
		m_nFileLogLineCnt = 0;
		m_dwCurFileSize				= 0;
		m_dwMaxLogFileSize			= DEFAULT_MAX_LOGFILE_SIZE;
		s_hLogFile = INVALID_HANDLE_VALUE;
		strncpy_s(m_szLogFileDirA, DEFAULT_LOGFILE_PATH, strlen(DEFAULT_LOGFILE_PATH) + 1);
		this->PrintModuleInit();
	}
public:
	static inline HINSTANCE GetCurrentModuleHandle(void);
	static inline LPCTSTR GetCurrentModuleVersion();
	static inline LPCTSTR GetCurrentModuleName(void);
	inline BOOL MakeDirectory(LPCTSTR lpszDestPath);
	inline BOOL MakeDirectoryA(LPCSTR lpszDestDirA);
#if (defined(UNICODE) || defined(_UNICODE))
	static inline BOOL UnicodeToMultiByte(const std::basic_string<WCHAR> &strSrc, std::string &strDst);
	static inline BOOL MultiByteToUnicode(const std::string& strSrc, std::basic_string<WCHAR> &strDst);
#endif //End defined(UNICODE)
	static inline UINT GetStringCodePage(const char* szData);
public:
	static TCHAR		s_szModuleFileName[_MAX_PATH];
	static HINSTANCE	s_hInst;			// 模块句柄
	static CISOPLog*	s_pThis;
	static DWORD		s_dwTlsIndex;
	static DWORD		s_dwTlsIndex_this;
	static HANDLE		s_hLogFile;			// 日志文件句柄
private:
	INT32				m_nFileLogLineCnt;	// 文件日志中的行号
	DWORD				m_dwCurFileSize;
	DWORD				m_dwMaxLogFileSize;
	CHAR				m_szLogFileDirA[MAX_PATH];	
	HMODULE				m_hLang;
public:
	static CISOPLog* CISOPLog::GetInstance(LPCTSTR pszCallFunName = NULL, LPCVOID pvThis = NULL)
	{
		if(s_pThis)
		{
			TlsSetValue(s_dwTlsIndex, (LPVOID)pszCallFunName);
			TlsSetValue(s_dwTlsIndex_this, (LPVOID)pvThis);
			return s_pThis;
		}
		s_hInst = reinterpret_cast<HINSTANCE>(GetCurrentModuleHandle());
		::GetModuleFileName(s_hInst, s_szModuleFileName, MAX_PATH - 1);
		static CISOPLog s_isopInstance;
		return &s_isopInstance;
	}

	class CISOPLogEnter
	{
	public:
#define MAX_LEAVE_FUNNAME_LENGTH 256
#define ENTER_FUNCTION_FLAG _T("→")
#define LEAVE_FUNCTION_FLAG _T("←")
		CISOPLogEnter(LPCTSTR pszCallFunName ,LPCTSTR pszFunction, LPCTSTR pszFuncSig,LPCVOID pvThis = 0, LPCSTR pszFuncSigA = NULL)
			:m_pvThis(pvThis), m_pszFunction(pszFunction), m_pszFuncSig(pszFuncSig), m_pszCallFunName(pszCallFunName), m_pszFuncSigA(pszFuncSigA)
		{
			CISOPLog::GetInstance(m_pszFunction, m_pvThis)->OutputLog(LEVEL_TRACE, m_pszCallFunName, NULL);
		}
		~CISOPLogEnter()
		{
			TCHAR szFunction[MAX_LEAVE_FUNNAME_LENGTH];
			_sntprintf(szFunction , MAX_LEAVE_FUNNAME_LENGTH - 1, _T("%s%s"), LEAVE_FUNCTION_FLAG, &m_pszFunction[OFFSET_LEAVE_FUNCTION_FLAG]);
			CISOPLog::GetInstance(szFunction , m_pvThis)->OutputLog(LEVEL_TRACE, m_pszFuncSig, NULL);
		}
	private:
		LPCVOID m_pvThis;
		LPCTSTR m_pszFunction;	  // function name					CTSGHeapVector::Add
		LPCTSTR m_pszFuncSig;	  // prototype +  param				BOOL CTSGHeapVector::Add(LPVOID)
		LPCSTR	m_pszFuncSigA;
		LPCTSTR m_pszCallFunName; // prototype +  param + value		BOOL CTSGHeapVector::Add(LPVOID = 0x00000000)		
	private:
		static const size_t OFFSET_LEAVE_FUNCTION_FLAG;
		DWORD	m_dwEBP;
	};
private:
	static inline LONGLONG GetTickCount64(void)
	{		
		__asm __emit 0x0F;
		__asm __emit 0x31;
	}
	inline void PrintModuleInit();
	inline void TSPrompt(LPCTSTR pszFormat, ...); //打印提示信息, 内部使用
	inline void OutputLog(LogLevel level, LPCTSTR pszFormat, va_list ap = NULL) throw();
	inline void WriteToLogFile(LogLevel level, LPCTSTR pszFileLogMsg, DWORD dwLen) throw();
public:
	inline	void			TSTrace(LPCTSTR pszFormat, ...){ OUTPUTLOG_FORLEVEL(LEVEL_TRACE, pszFormat); }
	inline	void			TSDebug(LPCTSTR pszFormat, ...){ OUTPUTLOG_FORLEVEL(LEVEL_DEBUG, pszFormat) }
	inline	void			TSInfo (LPCTSTR pszFormat, ...){ OUTPUTLOG_FORLEVEL(LEVEL_INFO, pszFormat) }
	inline	void			TSWarn (LPCTSTR pszFormat, ...){ OUTPUTLOG_FORLEVEL(LEVEL_WARN, pszFormat) }
	inline	void			TSError(LPCTSTR pszFormat, ...){ OUTPUTLOG_FORLEVEL(LEVEL_ERROR, pszFormat) }
	inline	void			TSFatal(LPCTSTR pszFormat, ...){ OUTPUTLOG_FORLEVEL(LEVEL_FATAL, pszFormat) }
	inline	void			TSLog(LPCTSTR pszLogFilePath,LPCTSTR pszFormat, ...); //打印提示信息, 内部使用
};

void CISOPLog::OutputLog(LogLevel level, LPCTSTR pszFormat, va_list ap)
{
	//SectionProtect sp(& _cs);
	LPTSTR _pszCallFunName = NULL;
	//#		Time	[PID] [TID] <模块名> 级别 信息\t\r\n
	//样例:
	//00000000\t00:00:00.000\t[012345] [012345] TRACE  <MyModule> User Msg\t\r\n
	TCHAR szWholeMsg[MAX_USERDATA_SIZE + 1] = {0};
	LPTSTR pWholeMsg = szWholeMsg;// pWholeMsg = szWholeMsg;
	LPTSTR pDebugViewMsg = NULL;
	LPTSTR pLevelMsg = NULL;
	LPTSTR pUserMsg = NULL;
	size_t  nDebugViewMsgoffset = 0;
	size_t  nLevelMsgoffset = 0;
	size_t  nUserMsgoffset = 0;
	//	size_t  nLen = 0;
	//pWholeMsg = szWholeMsg; // new TCHAR[MAX_USERDATA_SIZE];
	//ZeroMemory(pWholeMsg, (MAX_USERDATA_SIZE + MAX_PRIVATEDATA_SIZE)*sizeof(TCHAR));
	//1. 加入前面的项, 即#		Time	[PID] [TID] 
	SYSTEMTIME nowtime = {0};
	static DWORD nCurProcessId = 0;
	if((DWORD)0 == nCurProcessId)
		nCurProcessId = (DWORD)GetCurrentProcessId();
	GetLocalTime(&nowtime);
	nDebugViewMsgoffset = _sntprintf(pWholeMsg, MAX_PRIVATEDATA_SIZE, _T("%08d\t%02d:%02d:%02d.%03d\t[%d] "),
		m_nFileLogLineCnt++,nowtime.wHour, nowtime.wMinute, nowtime.wSecond, nowtime.wMilliseconds, nCurProcessId );
	pDebugViewMsg = pWholeMsg + nDebugViewMsgoffset; //向OutputDebugString传递的字符串的首地址
	//2.加入 [TID] 
	nLevelMsgoffset = _sntprintf(pDebugViewMsg, MAX_PRIVATEDATA_SIZE, _T("[%d] "), GetCurrentThreadId());
	pLevelMsg = pDebugViewMsg + nLevelMsgoffset; //级别字符串的首地址
	//nUserMsgoffset =  _tcslen(_T("TRACE  ")) + _tcslen(GetCurrentrentModuleName()) + _tcslen(_T("<> "));
	//3. 加入日志级别和模块名
	//nLen = nUserMsgoffset;
	switch(level)
	{
	case LEVEL_TRACE:	nUserMsgoffset = _sntprintf(pLevelMsg, MAX_PRIVATEDATA_SIZE, _T("TRACE  <%s> "), GetCurrentModuleName());	break;
	case LEVEL_DEBUG:	nUserMsgoffset = _sntprintf(pLevelMsg, MAX_PRIVATEDATA_SIZE, _T("DEBUG  <%s> "), GetCurrentModuleName());	break;
	case LEVEL_INFO:	nUserMsgoffset = _sntprintf(pLevelMsg, MAX_PRIVATEDATA_SIZE, _T("INFO   <%s> "), GetCurrentModuleName());	break;
	case LEVEL_WARN:	nUserMsgoffset = _sntprintf(pLevelMsg, MAX_PRIVATEDATA_SIZE, _T("WARN   <%s> "), GetCurrentModuleName());	break;
	case LEVEL_ERROR:	nUserMsgoffset = _sntprintf(pLevelMsg, MAX_PRIVATEDATA_SIZE, _T("ERROR  <%s> "), GetCurrentModuleName());	break;
	case LEVEL_FATAL:	nUserMsgoffset = _sntprintf(pLevelMsg, MAX_PRIVATEDATA_SIZE, _T("FATAL  <%s> "), GetCurrentModuleName());	break;
	case LEVEL_PROMPT:	nUserMsgoffset = _sntprintf(pLevelMsg, MAX_PRIVATEDATA_SIZE, _T("PROMPT <%s> "), GetCurrentModuleName());	break;
	default:
		return;
	}
	//nUserMsgoffset = _tcslen(pLevelMsg);
	pUserMsg = pLevelMsg + nUserMsgoffset; //用户信息的字符串的首地址
	//4.增加打印  00000000\t00:00:00.000\t[012345] [012345] TRACE  <MyModule> [CALLFUNNAME]
	size_t nFunNameLen = 0;
	LPTSTR s_pszCallFunName = NULL;
	LPDWORD s_pdwThis = 0;
	pUserMsg[0] = _T('[');
	__try
	{
		s_pszCallFunName = (LPTSTR)TlsGetValue(s_dwTlsIndex);
		s_pdwThis = (LPDWORD) TlsGetValue(s_dwTlsIndex_this);
		if (NULL == s_pszCallFunName)
			nFunNameLen = 0;
		else
		{
			nFunNameLen = _tcslen(s_pszCallFunName);
			memcpy(&pUserMsg[1], s_pszCallFunName, (nFunNameLen) * sizeof(TCHAR));
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		nFunNameLen = _tcslen(_T("!!!"));
		memcpy(&pUserMsg[1], _T("!!!"), (nFunNameLen) * sizeof(TCHAR));
	}
	pUserMsg[nFunNameLen + 1] = _T(']');
	pUserMsg[nFunNameLen + 2] = _T(' ');
	pUserMsg += nFunNameLen + 3;
	if(NULL != s_pdwThis)	//
	{
		INT s_nThisLen = 0;
		DWORD s_dwThis = 0;
		__try
		{
			memcpy(&s_dwThis,&s_pdwThis, sizeof(DWORD));
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			s_dwThis =(DWORD)-1;
		}
		pUserMsg[0] = _T('[');
		_sntprintf(&pUserMsg[1],MAX_PRIVATEDATA_SIZE,_T("0x%08x"), s_dwThis);
		s_nThisLen = (INT)_tcslen(pUserMsg);
		pUserMsg[s_nThisLen] = _T(']');
		pUserMsg[s_nThisLen + 1] = _T(' ');
		pUserMsg += s_nThisLen + 2;
	}
#define FINALFLAG _T("\t\r\n")
	//	size_t  nFinalFlagLen = _tcslen(_T("\t\r\n"));
	__try
	{
		if(NULL == ap)
			nUserMsgoffset = _sntprintf(pUserMsg, MAX_USERDATA_SIZE - (pUserMsg-pWholeMsg) - 64, _T("%s"), pszFormat);
		else
			nUserMsgoffset = _vsntprintf(pUserMsg, MAX_USERDATA_SIZE - (pUserMsg-pWholeMsg) - 64, pszFormat, ap);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		nUserMsgoffset = _sntprintf(pUserMsg, MAX_USERDATA_SIZE - (pUserMsg-pWholeMsg) - 64, _T("(!!!)Access Error, Invalid Memory Address : 0x%p, ExceptionCode : 0x%08x"), pszFormat, GetExceptionCode());
	}
	if((size_t)-1 == nUserMsgoffset)
		nUserMsgoffset = _tcslen(pUserMsg);
	LPTSTR pEnd = pUserMsg + nUserMsgoffset - 1;
	while( '\t' == *pEnd ||  '\r' == *pEnd || '\n' == *pEnd)//清除最后的制表符, 回车和换行
		*pEnd-- = 0;
	//_tcsncat(++pEnd, FINALFLAG , nFinalFlagLen); //在后面加\t\r\n
	LARGE_INTEGER li = {0};
	li.QuadPart = GetTickCount64();
	nUserMsgoffset = _sntprintf(++pEnd, 32, _T("\t'%hu '%hu%s"), HIWORD(li.LowPart), LOWORD(li.LowPart), FINALFLAG); 
	pEnd += nUserMsgoffset;
	//_tcsncat(++pEnd, FINALFLAG , nFinalFlagLen); //在后面加\t\r\n
	OutputDebugString(pDebugViewMsg);
	WriteToLogFile(level, pWholeMsg, DWORD(pEnd - pWholeMsg));
}

void CISOPLog::WriteToLogFile(LogLevel level, LPCTSTR pszFileLogMsg, DWORD dwLen)
{
	//该进程第一次打文件日志, 还没有打开日志文件
	if(s_hLogFile == INVALID_HANDLE_VALUE)
	{
		//从日志中得到模块名
		DWORD dwAttri = GetFileAttributesA(m_szLogFileDirA);
		if(dwAttri != INVALID_FILE_ATTRIBUTES && dwAttri & FILE_ATTRIBUTE_DIRECTORY) //目录存在
			;
		else //目录不存在, 创建目录
		{
			if(!MakeDirectoryA(m_szLogFileDirA))
			{
				if(!GetTempPathA(_MAX_PATH, m_szLogFileDirA))
					return;
#if (defined(UNICODE) || defined(_UNICODE))
				sprintf_s(m_szLogFileDirA, _MAX_PATH, "%s", m_szLogFileDirA);
#else
				_snprintf(m_szLogFileDirA, _MAX_PATH, "%s", m_szLogFileDirA);
#endif
				if(!MakeDirectoryA(m_szLogFileDirA))
					return;
			}
		}
		//构造完整路程加文件名
		TCHAR szLogFilePathName[MAX_PATH] = {0};
		DWORD dwPID = GetCurrentProcessId();
		LPCTSTR pszCurModuleName = GetCurrentModuleName();
		LPCTSTR pszLogFilePath = NULL;
#if defined(UNICODE) || defined(_UNICODE)
		std::basic_string<WCHAR> strLogFilePathW;
		MultiByteToUnicode(m_szLogFileDirA, strLogFilePathW);
		pszLogFilePath = strLogFilePathW.c_str();
#else
		pszLogFilePath = m_szLogFileDirA;
#endif
		_sntprintf(szLogFilePathName, _MAX_PATH, _T("%s%s[%d].log"), pszLogFilePath, pszCurModuleName, dwPID); //no reach
		//打开日志文件
		s_hLogFile =  CreateFile(szLogFilePathName, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0 , NULL);
		if(INVALID_HANDLE_VALUE == s_hLogFile)
		{
			DWORD dwErrCode = GetLastError();
			if((DWORD)ERROR_DISK_FULL == dwErrCode)
			{
#ifdef _DEBUG				
				MessageBox(NULL, _T("Disk is full."), s_szModuleFileName, MB_OK);
#endif
				return;
			}
			else if((DWORD)ERROR_ACCESS_DENIED == dwErrCode)
			{
				//尝试在临时目录下建文件
				if(!GetTempPathA(_MAX_PATH, m_szLogFileDirA))
					return;				
#if (defined(UNICODE) || defined(_UNICODE))
				_snprintf(m_szLogFileDirA, _MAX_PATH, "%sTSLOG\\", m_szLogFileDirA);
#else
				_snprintf(m_szLogFileDirA, _MAX_PATH, "%sTSLOG\\", m_szLogFileDirA);
#endif
				DWORD dwPID = GetCurrentProcessId();
				LPCTSTR pszCurModuleName = GetCurrentModuleName();
				LPCTSTR pszLogFilePath = NULL;
#if defined(UNICODE) || defined(_UNICODE)
				std::basic_string<WCHAR> strLogFilePathW;
				MultiByteToUnicode(m_szLogFileDirA, strLogFilePathW);
				pszLogFilePath = strLogFilePathW.c_str();
#else
				pszLogFilePath = m_szLogFileDirA;
#endif
				_sntprintf(szLogFilePathName, _MAX_PATH, _T("%s\\%s[%d].log"), pszLogFilePath, pszCurModuleName, dwPID); //no reach
				s_hLogFile =  CreateFile(szLogFilePathName, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0 , NULL);
				if(INVALID_HANDLE_VALUE == s_hLogFile)
					return;				
			}
			else
				return;
		}
	}
	DWORD dwByteWritten = 0;
#if (defined(UNICODE) || defined(_UNICODE))
	//可以先把Unicode字符串转成ANSI字符串
	CHAR szBufferA[MAX_USERDATA_SIZE + MAX_PRIVATEDATA_SIZE + 1] = {0};	//调用频繁
	INT nLen = 0;
	nLen = WideCharToMultiByte(936, 0, pszFileLogMsg, -1,NULL, 0 ,NULL, NULL);
	if(0 >= nLen)
		return;
	else if(MAX_USERDATA_SIZE + MAX_PRIVATEDATA_SIZE  < nLen)
		return;
	dwLen = WideCharToMultiByte(936, 0, pszFileLogMsg, -1, szBufferA, nLen,NULL,NULL);	
	if(0 >= dwLen)
		return;
	dwLen--;
	if(MAX_USERDATA_SIZE + 1 < dwLen)
		dwLen = MAX_USERDATA_SIZE + 1;
	//磁盘满停止打印
	if(!WriteFile(s_hLogFile, szBufferA, dwLen, &dwByteWritten, NULL))
	{
		if((DWORD)ERROR_DISK_FULL == GetLastError())
		{
		}
		return;
	}
#else
	if(INVALID_HANDLE_VALUE != s_hLogFile)
	{
		SetFilePointer(s_hLogFile,0, 0, FILE_END);
		WriteFile(s_hLogFile,pszFileLogMsg, dwLen, &dwByteWritten, NULL);
		CloseHandle(s_hLogFile);
		s_hLogFile = INVALID_HANDLE_VALUE;
	}
	//磁盘满停止打印
	if(!WriteFile(s_hLogFile, pszFileLogMsg, dwLen, &dwByteWritten, NULL))
	{
		if((DWORD)ERROR_DISK_FULL == GetLastError())
		{
			m_bFileLog = FALSE;
			WritePrivateProfileString(_T("Output"), _T("FileLog"), _T("OFF"), __TTSLOG_CONFIG_FILE_PATH);
		}
		return;
	}
#endif
	//如果文件大小超过指定大小, 则把之前所有的日志文件改名
	//static volatile DWORD dwSize = 0;	//GetFileSize(m_hLogFile, NULL);
	if(LEVEL_PROMPT != level)
		m_dwCurFileSize += dwByteWritten; 
	if(m_dwCurFileSize >= m_dwMaxLogFileSize * 1 * KBTYE) //m_dwMaxLogSize的单位是KB
	{
		m_dwCurFileSize = 0;
		FlushFileBuffers(s_hLogFile);
		CloseHandle(s_hLogFile);
		TCHAR szLogFilePathName[MAX_PATH] = {0};
		memset(szLogFilePathName, 0x00, sizeof(szLogFilePathName));
		//构造完整路程加文件名
		//TCHAR szLogFilePathName[MAX_PATH] = {0};
		memset(szLogFilePathName, 0x00, sizeof(szLogFilePathName));
		DWORD dwPID = GetCurrentProcessId();
		LPCTSTR pszCurModuleName = GetCurrentModuleName();
		LPCTSTR pszLogFilePath = NULL;
#if defined(UNICODE) || defined(_UNICODE)
		std::basic_string<WCHAR> strLogFilePathW;
		MultiByteToUnicode(m_szLogFileDirA, strLogFilePathW);
		pszLogFilePath = strLogFilePathW.c_str();
#else
		pszLogFilePath = m_szLogFileDirA;
#endif
		_sntprintf(szLogFilePathName, _MAX_PATH, _T("%s\\%s[%d].log"), pszLogFilePath, pszCurModuleName, dwPID); //no reach
		//打开日志文件
		s_hLogFile =  CreateFile(szLogFilePathName, GENERIC_WRITE, FILE_SHARE_WRITE|FILE_SHARE_READ, NULL,CREATE_ALWAYS, 0,NULL);
		if(s_hLogFile)
			PrintModuleInit();
	}
}

void CISOPLog::TSPrompt(LPCTSTR pszFormat, ...)
{
	TlsSetValue(s_dwTlsIndex, NULL);
	TlsSetValue(s_dwTlsIndex_this, NULL);
	va_list ap; 
	va_start(ap, pszFormat);
	OutputLog(LEVEL_PROMPT, pszFormat, ap);
	va_end(ap);	
}

void CISOPLog::PrintModuleInit()
{
	//
	TCHAR szData[MAX_USERDATA_SIZE + 1] = {0};
	_sntprintf_s(szData, MAX_USERDATA_SIZE, _T("Module Loaded. Entry Address(0x%08x), Path = %s"),
		(ULONG)(ULONG_PTR)GetCurrentModuleHandle(), s_szModuleFileName);
	TSPrompt(szData);
	LPCTSTR pszVertionType = NULL;
#if (defined(UNICODE) || defined(_UNICODE))
	pszVertionType =  L"'UNICODE'";
#else
	pszVertionType = "'MBCS'";
#endif
	TSPrompt(_T("BuildType : %s, FileVersionInfo : { %s } Rebuild Time : %s"), 
		pszVertionType, GetCurrentModuleVersion(), __TTIMESTAMP__);
}

HINSTANCE CISOPLog::GetCurrentModuleHandle(void)
{
	static HINSTANCE hCurrentModule = 0;
	if(NULL == hCurrentModule)
	{
		MEMORY_BASIC_INFORMATION m = { 0 };
		VirtualQuery(&hCurrentModule, &m, sizeof(MEMORY_BASIC_INFORMATION));
		hCurrentModule = (HINSTANCE) m.AllocationBase;
	}
	return hCurrentModule;
}

LPCTSTR CISOPLog::GetCurrentModuleVersion()
{
#define CB_FILE_VERSION 0x168
	HINSTANCE s_hInst = reinterpret_cast<HINSTANCE>(GetCurrentModuleHandle());
	TCHAR szModuleFileName[_MAX_PATH] = {0};
	LPTSTR pszInfoBuf = NULL;
	VS_FIXEDFILEINFO* pvi = NULL;
	UINT  cbTranslate = 0;
	static TCHAR szVerInfo[CB_FILE_VERSION] = {0};
	::GetModuleFileName( s_hInst,szModuleFileName , MAX_PATH);
	DWORD dwInfoSize = 0, dwHandle = 0;
	dwInfoSize = GetFileVersionInfoSize(szModuleFileName,&dwHandle);
	if((DWORD)0 == dwInfoSize)
		return NULL;
	pszInfoBuf = new TCHAR[dwInfoSize];
	if(NULL == pszInfoBuf)
		return NULL;
	if(GetFileVersionInfo(szModuleFileName,0,dwInfoSize,pszInfoBuf))
	{
		if(VerQueryValue(pszInfoBuf, _T("\\"),(LPVOID*)&pvi,&cbTranslate))
		{
			_sntprintf(szVerInfo,CB_FILE_VERSION, _T("FileVersion : %d.%d.%d.%d [ FileVersionMS : 0x%08x, FileVersionLS : 0x%08x, ProductVersionMS : 0x%08x, ProductVersionLS : 0x%08x, ") 
				_T("Signature : 0x%08x, FileFlags : 0x%08x, FileFlagsMask : 0x%08x, FileOS : 0x%08x, ")
				_T("FileType : 0x%08x, FileSubType : 0x%08x, FileDateMS : 0x%08x, FileDateLS : 0x%08x ]") ,
				HIWORD(pvi->dwFileVersionMS), LOWORD(pvi->dwFileVersionMS), HIWORD(pvi->dwFileVersionLS), LOWORD(pvi->dwFileVersionLS),
				pvi->dwFileVersionMS, pvi->dwFileVersionLS, pvi->dwProductVersionMS, pvi->dwProductVersionLS ,
				pvi->dwSignature, pvi->dwFileFlags, pvi->dwFileFlagsMask, pvi->dwFileOS,
				pvi->dwFileType, pvi->dwFileSubtype, pvi->dwFileDateMS, pvi->dwFileDateLS);
		}
	}
	delete pszInfoBuf;
	pszInfoBuf = NULL;
	return szVerInfo[0] ? szVerInfo : NULL;
}

LPCTSTR CISOPLog::GetCurrentModuleName(void)
{
	static TCHAR szCurModuleName[_MAX_FNAME] = {0};
	if('\0' != szCurModuleName[0])
		return szCurModuleName;
	HINSTANCE s_hInst = reinterpret_cast<HINSTANCE>(GetCurrentModuleHandle());
	TCHAR szModuleFileName[_MAX_PATH] = {0};
	LPTSTR pszModuleFileName = szModuleFileName;
	::GetModuleFileName( s_hInst,szModuleFileName , MAX_PATH);
	pszModuleFileName += _tcslen(szModuleFileName);
	while ('\\' != *(--pszModuleFileName)) ;
	pszModuleFileName++;
	_tcsncpy(szCurModuleName, pszModuleFileName,_MAX_FNAME); //
	pszModuleFileName = szCurModuleName;			//去除 后缀名
	while ( '\0' != *pszModuleFileName)
	{
		pszModuleFileName++;
		if( '.'== *pszModuleFileName)
		{
			*pszModuleFileName = '\0';
			break;
		}
	}
	return szCurModuleName;
}

BOOL CISOPLog::MakeDirectory(LPCTSTR lpszDestPath) //no other version of MakeDirectoryA
{
	LPTSTR lpszHead = (LPTSTR)lpszDestPath;
	LPTSTR lpszDir = (LPTSTR)lpszDestPath;
	TCHAR szTempDir[_MAX_PATH] = {0};
	DWORD dwAttributes = 0;
	if (NULL == lpszDir)
		return FALSE;
	while ( '\0' != *(++lpszDir))
	{
		if ( '\\' == *lpszDir)
		{
			memcpy(szTempDir, lpszHead, (lpszDir - lpszHead) * sizeof(TCHAR));
			szTempDir[lpszDir - lpszHead] = '\0';
			dwAttributes = GetFileAttributes(szTempDir);
			if( INVALID_FILE_ATTRIBUTES == dwAttributes || !(FILE_ATTRIBUTE_DIRECTORY & dwAttributes))
				if(!CreateDirectory(szTempDir, NULL))
					return FALSE;
		}
	}
	return TRUE;
}

BOOL CISOPLog::MakeDirectoryA(LPCSTR lpszDestDirA)
{
	LPSTR lpszHead = (LPSTR)lpszDestDirA;
	LPSTR lpszDir = (LPSTR)lpszDestDirA;
	CHAR szTempDir[_MAX_PATH] = {0};
	DWORD dwAttributes = 0;
	if (NULL == lpszDir)
		return FALSE;
	while ( '\0' != *(++lpszDir))
	{
		if ( '\\' == *lpszDir)
		{
			memcpy(szTempDir, lpszHead, (lpszDir - lpszHead) * sizeof(CHAR));
			szTempDir[lpszDir - lpszHead] = '\0';
			dwAttributes = GetFileAttributesA(szTempDir);
			if((DWORD)0xFFFFFFFF == dwAttributes || !(FILE_ATTRIBUTE_DIRECTORY & dwAttributes))
				if(!CreateDirectoryA(szTempDir, NULL))
					return FALSE;
		}
	}
	return TRUE;
}

#if (defined(UNICODE) || defined(_UNICODE))
BOOL CISOPLog::MultiByteToUnicode(const std::string& strSrc, std::basic_string<WCHAR> &strDst)
{
	INT nSrcAnsiLen = 0;
	nSrcAnsiLen = (INT)strSrc.length();
	if(0 == nSrcAnsiLen)
		return TRUE;
	UINT code_page = GetStringCodePage(strSrc.c_str());
	INT nLen = 0;
	nLen = MultiByteToWideChar(code_page,0,strSrc.c_str(),-1,NULL,0);
	if(0 == nLen)
	{
		code_page = 0;
		nLen = MultiByteToWideChar(code_page,0,strSrc.c_str(),-1,NULL,0);
	}
	if(MAX_USERDATA_SIZE < nLen)
		nLen = MAX_USERDATA_SIZE;
	WCHAR szBuf[MAX_USERDATA_SIZE + 1] = {0};
	LPWSTR pszBuf = szBuf;
	INT nResult = 0;
	nResult = MultiByteToWideChar(code_page,0,strSrc.c_str(), -1, pszBuf,nLen);
	if(nResult <= 0 )
		return FALSE;	
	if(pszBuf)
		strDst = pszBuf;
	return TRUE;
}
#endif

UINT CISOPLog::GetStringCodePage(const char* szData)
{
	UINT code_page = 0;//default value
	if(NULL == szData || NULL == s_pThis) //is not main thread , can not print
		return code_page;
	IMultiLanguage2*  pMLang = NULL;
	HRESULT hr = S_FALSE;
	HRESULT hresult = S_FALSE;    
	int srclen = (int)strlen(szData); 
	DetectEncodingInfo dei[8] = {0};		
	int dei_num = 8;
	INT confidence = 0;
	typedef HRESULT (STDAPICALLTYPE *_pfnDllGetClassObject)(REFCLSID , REFIID , LPVOID*);
	_pfnDllGetClassObject pfnDllGetClassObject = (_pfnDllGetClassObject)GetProcAddress(s_pThis->m_hLang, "DllGetClassObject");
	__try
	{	 
		if(pfnDllGetClassObject)
		{
			CoInitialize(NULL);
			IClassFactory* pCF = NULL;
			IMultiLanguage2* pMLang = NULL;
			hr = (*pfnDllGetClassObject)(CLSID_CMultiLanguage, IID_IClassFactory, (LPVOID*) &pCF);
			if(pCF && SUCCEEDED(hr))
			{
				hr = pCF->CreateInstance(NULL, IID_IMultiLanguage2, (void **)&pMLang);	
			}
			if(pMLang && SUCCEEDED(hr))
			{
				hresult = pMLang->DetectInputCodepage(MLDETECTCP_NONE, 0 , (char *)szData, &srclen , dei , &dei_num);  
				if (SUCCEEDED(hresult))
				{
					confidence = 50;
					for (int i = 0 ; i < dei_num; i++)
					{
						code_page = dei[i].nCodePage;
						if(936 == code_page)
							break;
						else if(confidence < dei[i].nConfidence)
						{
							confidence = dei[i].nConfidence;						
						}
					}
				}
				pMLang->Release();
			}
			CoUninitialize();
		}//pfnDllGetClassObject
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		s_pThis->TSPrompt(_T(" IMultiLanguage2 Fatal. code_page = %lu, datalen = %d, CoCreateInstance = %lu, pMLang = %lu, dei_num = %d, confidence = %d, DetectInputCodepage = %d "), 
			code_page, srclen, hr, pMLang, dei_num, confidence, hresult);
	}
	return code_page;
};

__declspec(selectany) TCHAR CISOPLog::s_szModuleFileName[_MAX_PATH]= {0};
__declspec(selectany) HINSTANCE CISOPLog::s_hInst = NULL;
__declspec(selectany) HANDLE CISOPLog::s_hLogFile = INVALID_HANDLE_VALUE;
__declspec(selectany) DWORD CISOPLog::s_dwTlsIndex = TLS_OUT_OF_INDEXES;
__declspec(selectany) DWORD CISOPLog::s_dwTlsIndex_this = TLS_OUT_OF_INDEXES;
__declspec(selectany) const size_t CISOPLog::CISOPLogEnter::OFFSET_LEAVE_FUNCTION_FLAG = _tcslen(LEAVE_FUNCTION_FLAG);
__declspec(selectany) CISOPLog* CISOPLog::s_pThis = CISOPLog::GetInstance();
#endif