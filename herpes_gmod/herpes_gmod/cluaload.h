// Herpes v4

#ifdef WIN32
 #pragma once
#endif

#ifndef ____cluaload__
#define ____cluaload__

#include "csdk.h"

struct lua_State;

struct LoadS {
    const char *s;
    size_t size;
};

typedef const char* ( *lua_Reader )( lua_State *L, void *data, size_t *size );
typedef int ( __cdecl* LuaLoad_t )( lua_State *L, lua_Reader reader, void *data, const char *chunkname );
typedef int ( __cdecl* LuaLoadBuffer_t ) ( lua_State *L, const char *buff, size_t sz, const char *name );

namespace hack
{
	void LoadScript( string path );
};

#endif //____cluaload__