#pragma once

#define SINGLETON_CLASS(classname) \
	private: \
	classname(){}; \
	public: \
	static classname* GetInstance() \
	{ \
	static classname s_classname; \
	return &s_classname;\
	};