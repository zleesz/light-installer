#pragma once

#define SINGLETON_CLASS(classname) \
	public: \
	static classname* GetInstance() \
	{ \
	static classname s_classname; \
	return &s_classname;\
	};

#define SINGLETON_CLASS2(classname) \
	private: \
	classname(){}; \
	SINGLETON_CLASS(classname)

#define GET_SINGLETON(classname) \
	classname::GetInstance()
// error code
// script parser
typedef enum enumSPErrorCode
{
	SP_ERROR_NO_ERROR = 0,
	SP_ERROR_BASE = 10000,
	SP_ERROR_FILE_NOT_FOUND,
	SP_ERROR_FILE_OPEN_FAILED,
	SP_ERROR_SECTION_TOKEN_MISSING,
}SPErrorCode;

// script operation
typedef enum _enumOpErrorCode
{
	OP_ERROR_NO_ERROR = 0,
	OP_ERROR_BASE = 10000,
	OP_ERROR_FILE_OPEN_FAILED,
	OP_ERROR_PARSER_BASE = 20000,
	OP_ERROR_PARSER_COPY_BASE = 21000,
	OP_ERROR_PARSER_DEL_BASE = 22000,
	OP_ERROR_PARSER_INCLUDE_BASE = 23000,
	OP_ERROR_PARSER_MOVE_BASE = 24000,
	OP_ERROR_PARSER_PATH_BASE = 25000,
	OP_ERROR_PARSER_REGISTER_BASE = 26000,
	OP_ERROR_PARSER_RMDIR_BASE = 27000,
	OP_ERROR_PARSER_SHELLEXCUTE_BASE = 28000,
	OP_ERROR_PARSER_VAR_BASE = 29000,
}OpErrorCode;