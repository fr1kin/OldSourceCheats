#include "ILuaCallback.h"

void* ILuaCallback::CreateLuaObject( void )
{
	OutputDebugStringA( "1" );
	ILuaObject* pObj = new ILuaObject();
	return pObj;
}

void ILuaCallback::DestroyLuaObject( ILuaObject* pObj )
{
	OutputDebugStringA( "2" );
	delete pObj;
}

void ILuaCallback::ErrorPrint( const char* pszMsg, bool bIDK )
{
	OutputDebugStringA( "3" );
}

void ILuaCallback::Msg( const char* pszMsg, bool bIDK )
{
	OutputDebugStringA( "4" );
}

void ILuaCallback::MsgColour( const char* pszMsg, void* cColor )
{
	OutputDebugStringA( "5" );
}

void ILuaCallback::LuaError( void* pLuaError )
{
	OutputDebugStringA( "6" );
}

void ILuaCallback::InterfaceCreated( ILuaInterface* pLua )
{
	OutputDebugStringA( "7" );
}
