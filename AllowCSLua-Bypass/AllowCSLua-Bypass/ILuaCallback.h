#ifndef __ilua__callback
#define __ilua__callback

#include "sdk.h"
#include "ILuaObject.h"

#ifndef LUA_SHIT
#define LUA_SHIT
struct lua_State;

typedef int (*Lua_Func) (lua_State *L);

class ILuaInterface;
class ILuaShared;
class ILuaObject;
#endif

class ILuaCallback
{
public:
	virtual void* CreateLuaObject( void ); //0
	virtual void DestroyLuaObject( ILuaObject* pObj ); //4
	virtual void ErrorPrint( const char*, bool ); //8
	virtual void Msg( const char* str, bool b );//12
	virtual void MsgColour(char  const*, void* );//16
	virtual void LuaError( void* );//20
	virtual void InterfaceCreated( ILuaInterface* pLua );//24
};

#endif //__ilua__callback