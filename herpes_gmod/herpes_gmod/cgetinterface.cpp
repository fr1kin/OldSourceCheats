// Herpes v4

#ifdef WIN32
 #pragma once
#endif

#include "csdk.h"

using namespace hack;

ILuaInterface* interfaces::GetInterface( int iInterface )
{
	CSysModule *sysModule			= Sys_LoadModule( MODULE_SHARED );
	CreateInterfaceFn sysFactory	= Sys_GetFactory( sysModule );

	CInterfaces::m_pLuaShared = ( ILuaShared* )sysFactory( "LuaShared001", NULL );
	return g_Interfaces.m_pLuaShared->GetLuaInterface( iInterface );
}