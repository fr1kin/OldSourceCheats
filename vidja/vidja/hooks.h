#ifndef __vidja_hooks_
#define __vidja_hooks_

#include "sdk.h"

#define ILuaShared__CreateLuaInterface____INDEX	4
#define ILuaShared__CloseLuaInterface____INDEX	5

#define ILuaInterface__Init____INDEX 47

enum LuaStates
{
	CLIENT,
	SERVER,
	MENU,
};

namespace vidja
{
	namespace Hooks
	{
		void Hook();
		ILuaInterface* __fastcall ILuaShared__CreateLuaInterface( void* thisptr, int edx, unsigned char iState, bool bArg2 );
		int __fastcall ILuaShared__CloseLuaInterface( void* thisptr, int edx, ILuaInterface* pLua );
		bool __fastcall ILuaInterface__Init( void* thisptr, int edx, void* pCallBack, bool bLoadJit );
	};
};

#endif