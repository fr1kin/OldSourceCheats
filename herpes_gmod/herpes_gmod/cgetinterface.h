// Herpes v4

#ifdef WIN32
 #pragma once
#endif

#ifndef ____cgetinterface__
#define ____cgetinterface__

#include "csdk.h"

enum
{
	STATE_CLIENT,
	STATE_SERVER,
	STATE_MENU,
};

namespace hack
{
	namespace interfaces
	{
		ILuaInterface* GetInterface( int iInterface );
	}
}

#endif //____cgetinterface__