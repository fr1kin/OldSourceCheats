// credits to jewlord ph)ne for GetCallAddress function

#include "sdk.h"
#include "hook.h"
#include "luadump.h"

#define LUA_DUMP

IGModDataPack*	ptr_DataPack	= NULL;
CVMTManager*	g_DataPackHook	= NULL;

typedef IGModDataPack* ( __cdecl* GetDataPackPointer_t )( void );

bool __cdecl new_BlockCSLua( void )
{
	return false;
}

bool GetDataPackPtr( void )
{
	DWORD dwDataPackCall = FindPattern( ( DWORD )GetModuleHandleA( "client.dll" ), "\xE8\x00\x00\x00\x00\x8B\x10\x8B\xC8\x8B\x42\x10\xFF\xD0\x84\xC0\x74\x06\x33\xC0", "x????xxxxxxxxxxxxxxx" );

	if( dwDataPackCall == NULL )
		return false;

	GetDataPackPointer_t GetDataPackPointer = GetCallAddress< GetDataPackPointer_t >( dwDataPackCall );

	ptr_DataPack = GetDataPackPointer();

	if( ptr_DataPack != NULL )
		return true;

	return false;
}

DWORD WINAPI MainThread( LPVOID lpArguments )
{
	while( GetModuleHandleA( "client.dll" ) == NULL || GetModuleHandleA( "lua_shared.dll" ) == NULL || GetModuleHandleA( "vstdlib.dll" ) == NULL )
		Sleep( 100 );
	
#ifndef LUA_DUMP
	if( GetDataPackPtr() )
	{
		static CVMTManager DataPackHook( ptr_DataPack, true );
		g_DataPackHook = &DataPackHook;

		DataPackHook.HookMethod( &new_BlockCSLua, 4 );
	}
#endif

#ifdef LUA_DUMP
	DetourCreateInterface();
#endif

	return NULL;
}

BOOL WINAPI DllMain( HINSTANCE hInstance, DWORD dwReasonOfCall, LPVOID lpReserved )
{
	if ( dwReasonOfCall == DLL_PROCESS_ATTACH )
	{
		DisableThreadLibraryCalls( ( HMODULE )hInstance );

		memset( dllPath, 0, MAX_PATH );
		
		if( GetModuleFileNameA( hInstance, dllPath, MAX_PATH ) != 0 )
		{
			for( int i = ( int )( strlen( dllPath ) ); i > 0; i-- )
			{
				if( dllPath[i] == '\\' )
				{
					dllPath[i + 1] = 0;
					break;
				}
			}
		}
		for( int i = 0; i < strlen( dllPath ); i++ )
		{
			if( dllPath[i] == '\\' )
				dllPath[i] = '/';
		}

		CreateThread( NULL, 0, ( LPTHREAD_START_ROUTINE )MainThread, NULL, 0, NULL );
	}

	return TRUE;
}