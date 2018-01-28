#pragma once

#ifndef __player______
#define __player______

#include "sdk.h"

extern IClientEntityList* ptr_EntList;

namespace CModule
{
	int IsDormant( lua_State* L );
	int DrawHitbox( lua_State* L );
	bool GetClientEntList();
};

#endif //__player______