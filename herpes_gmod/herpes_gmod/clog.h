// Herpes v4
// From Herpes CSS

#ifdef WIN32
 #pragma once
#endif

#ifndef __H_CLOG_
#define __H_CLOG_

#include "CSDK.h"

using namespace std;

class CLog
{
public:
	void Create( const char *czFileName );
	void Add( PCHAR czTest, ... );
	void GetFilePath( HMODULE hmModule );
	char FILE_PATH[100];
private:
	char FILE_NAME[100];
};
extern CLog g_Log;

#endif //__H_CLOG_