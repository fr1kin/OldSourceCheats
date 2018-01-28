typedef unsigned long CRC32_t;

typedef void ( *CRC32_Init_t )( CRC32_t* );
typedef void ( *CRC32_ProcessBuffer_t )( CRC32_t*, const void*, int );
typedef void ( *CRC32_Final_t )( CRC32_t* );

extern CRC32_Init_t				CRC32_Init;
extern CRC32_ProcessBuffer_t	CRC32_ProcessBuffer;
extern CRC32_Final_t			CRC32_Final;

#ifndef __vidja_sdk_
#define __vidja_sdk_

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <direct.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <Strsafe.h>
#include <sys/stat.h>
#include <Psapi.h>
#include <string.h>

#include "vmthooks.h"

#include "main.h"

#include "luainterface.h"
#include "luashared.h"
#include "hlclient.h"
#include "input.h"
#include "usercmd.h"
#include "luaobject.h"
#include "checksummd5.h"

#include "memory.h"

#include "hooks.h"
#include "luafuncs.h"
#include "clientfuncs.h"

#pragma comment( lib, "User32.lib" )
#pragma comment( lib, "Psapi.lib" )

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

#define _vidja_DEBUGMODE

extern CMemory* g_pClientDll;
extern CMemory* g_pEngineDll;
extern CMemory* g_pLuaSharedDll;
extern CMemory* g_pVidjaDll;

namespace vidja
{
	class Interfaces
	{
	public:
		static CInput* Input;
		static ILuaShared* LuaShared;
		static ILuaInterface* LuaClient;
		static ILuaInterface* LuaServer;
		static ILuaInterface* LuaMenu;
	};
	void DoError( const char* pszMsg, ... );
	void DebugMessage( const char* pszMsg, ... );
	extern const char* m_pszGetLuaPath;
#ifdef _vidja_DEBUGMODE
	bool DebugCheckPointers();
#endif
};

#endif