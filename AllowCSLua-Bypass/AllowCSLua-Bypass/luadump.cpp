#include <fstream>
#include <iostream>
#include <time.h>

#include <direct.h>

#include "luadump.h"
#include "detourxs.h"

#include "sdk.h"
#include "hook.h"

#define CLIENT_DLL
#define GAME_DLL

#include <cdll_int.h>
#include <filesystem.h>
#include <game/client/cdll_client_int.h>

ILuaInterface*	ptr_Lua_Client		= NULL;
ILuaInterface*	ptr_Lua_Menu		= NULL;
ILuaShared*		ptr_LuaShared		= NULL;

bool			bRSHasHooked_Client	= false;
bool			bRSHasHooked_Menu	= false;

CVMTHook		g_RunStringEx_Client;
CVMTHook		g_RunStringEx_Menu;
CVMTHook		g_CreateLuaInterface;
CVMTHook		g_CloseLuaInterface;

void CreateDirectories( const char* path )
{
	char cpath[300];
	strcpy_s( cpath, path );
	for( int i = 0; i < strlen( cpath ); i++ )
	{
		if( cpath[i] == '\\' )
			cpath[i] = '/';
	}
	std::string str = cpath;
	std::string cur = "";
	int pos = 0;
	int orl = 0;
	do
	{
		if( orl > 500 )
			break;
		pos = str.find( "/" );
		if( pos == -1 && str != "" )
		{
			cur.append( str );
			_mkdir( cur.c_str() );
			break;
		}
		else if( pos == -1 )
			break;

		std::string folder = str.substr( 0, pos );
		str = str.substr( pos + 1 );
		cur.append( folder + "/" );
		_mkdir( cur.c_str() );
		orl++;
	}
	while( true );
}

//void* thisptr, int edx, 
bool __stdcall new_RunStringEx_Menu( const char* path, const char* arg2, const char* code, bool arg4, bool arg5, bool arg6 )
{
	ILuaInterface* pLua;
	__asm
	{
		mov pLua, ecx
	}

	OutputDebugStringA( path );

	// Format the full path.
	std::string fullPath = dllPath;
	fullPath.append( "dump/" );
	fullPath.append( path );

    char node[ 100 ];
    char dir[ _MAX_DIR ];
    char fname[ _MAX_FNAME ];
    char ext[ _MAX_EXT ];

	// Split the path and folders apart
	_splitpath_s( fullPath.c_str(), node, dir, fname, ext );
	
	// If there is no extension then make one
	if( ext[0] == '\0' )
		fullPath.append( ".lua" );

	// Create folders
	char fullFolderPath[258];
	strcpy_s( fullFolderPath, node );
	strcat_s( fullFolderPath, dir );

	CreateDirectories( fullFolderPath );

	std::ofstream file;

	file.open( fullPath, std::ios::out | std::ios::app | std::ios::binary );
	
	if( file.is_open() )
		file << code << "\n\n\n\n";

	file.close();

	g_RunStringEx_Menu.UnHook();
	bool ret = pLua->RunStringEx( path, arg2, code, arg4, arg5, arg6 );
	g_RunStringEx_Menu.ReHook();

	return ret;
}

bool __stdcall new_RunStringEx_Client( const char* path, const char* arg2, const char* code, bool arg4, bool arg5, bool arg6 )
{
	ILuaInterface* pLua;
	__asm
	{
		mov pLua, ecx
	}

	OutputDebugStringA( path );

	// Format the full path.
	std::string fullPath = dllPath;
	fullPath.append( "dump/" );
	fullPath.append( path );

    char node[ 100 ];
    char dir[ _MAX_DIR ];
    char fname[ _MAX_FNAME ];
    char ext[ _MAX_EXT ];

	// Split the path and folders apart
	_splitpath_s( fullPath.c_str(), node, dir, fname, ext );
	
	// If there is no extension then make one
	if( ext[0] == '\0' )
		fullPath.append( ".lua" );

	// Create folders
	char fullFolderPath[258];
	strcpy_s( fullFolderPath, node );
	strcat_s( fullFolderPath, dir );

	CreateDirectories( fullFolderPath );

	std::ofstream file;

	file.open( fullPath, std::ios::out | std::ios::app | std::ios::binary );
	
	if( file.is_open() )
		file << code << "\n\n\n\n";

	file.close();

	g_RunStringEx_Client.UnHook();
	bool ret = pLua->RunStringEx( path, arg2, code, arg4, arg5, arg6 );
	g_RunStringEx_Client.ReHook();

	return ret;
}

ILuaInterface*		ptr_Lua		= NULL;

CVMTHook			g_Init;

bool __stdcall new_Init( ILuaCallback* call, bool b )
{
	ILuaInterface* pLua;
	__asm
	{
		mov pLua, ecx
	}

	char msg1[300];
	sprintf( msg1, "Init Called [0x%X] = %s", ( DWORD )call, b ? "true" : "false" );
	OutputDebugStringA( msg1 );

	call->Msg( "Hi there", true );

	g_Init.UnHook();
	bool ret = pLua->Init( call, b );
	g_Init.ReHook();

	return ret;
}

int __stdcall new_CreateLuaInterface( unsigned char state, bool arg2 )
{
	g_CreateLuaInterface.UnHook();
	int ret = ptr_LuaShared->CreateLuaInterface( state, arg2 );
	g_CreateLuaInterface.ReHook();

	char msg1[300];
	sprintf( msg1, "New Interface Loaded: %i (%s)", (int)state, arg2 ? "true" : "false" );
	OutputDebugStringA( msg1 );

	static bool bHasDetoured = false;

	if( ( int )( state ) == 0 || ( int )( state ) == 2 )
	{
		if( bHasDetoured == false )
		{
			ptr_Lua = ptr_LuaShared->GetLuaInterface( state );

			g_Init.Hook( ( DWORD )&new_Init, ( PDWORD )ptr_Lua, 46 );

			bHasDetoured = true;
		}
	}

	/*
	if( ( int )( state ) == 0 ) // CLIENT
	{
		ptr_Lua_Client = ptr_LuaShared->GetLuaInterface( state );

		char msg[300];
		sprintf( msg, "New client Interface : %i (0x%X)", (int)state, ( DWORD )ptr_Lua_Client );
		OutputDebugStringA( msg );

		//DetourRunString_Client();
	}
	else if( ( int )( state ) == 2 ) // MENU
	{
		ptr_Lua_Menu = ptr_LuaShared->GetLuaInterface( state );

		char msg[300];
		sprintf( msg, "New Menu Interface : %i (0x%X)", (int)state, ( DWORD )ptr_Lua_Menu );
		OutputDebugStringA( msg );

		DetourRunString_Menu();
	}
	*/

	return ret;
}

int __stdcall new_CloseLuaInterface( ILuaInterface* pLua )
{
	if( ( DWORD )pLua == ( DWORD )ptr_Lua_Client )
	{
		OutputDebugStringA( "Interface client is closing, nulling pointer and unhooking..." );
		
		if( bRSHasHooked_Client )
		{
			g_RunStringEx_Client.UnHook();
			bRSHasHooked_Client = false;
		}

		ptr_Lua_Client = NULL;
	}
	else if( ( DWORD )pLua == ( DWORD )ptr_Lua_Menu )
	{
		OutputDebugStringA( "Interface menu is closing, nulling pointer and unhooking..." );
		
		if( bRSHasHooked_Menu )
		{
			g_RunStringEx_Menu.UnHook();
			bRSHasHooked_Menu = false;
		}

		ptr_Lua_Menu = NULL;
	}
	
	g_CloseLuaInterface.UnHook();
	int ret = ptr_LuaShared->CloseLuaInterface( pLua );
	g_CloseLuaInterface.ReHook();

	return ret;
}

//\x8B\x0D\x00\x00\x00\x00\x8B\x31\x8B\x10\x8B\xC8\x8B\x42\x28, xx????xxxxxxxxx - Client Interface

bool GetLuaShared()
{
	if( ptr_LuaShared == NULL )
	{
		CSysModule *sysModule = Sys_LoadModule( "lua_shared.dll" );
		CreateInterfaceFn sysFactory = Sys_GetFactory( sysModule );

		ptr_LuaShared = ( ILuaShared* )sysFactory( "LuaShared002", NULL );

		return true;
	}
	return false;
}

typedef int ( *UnRef_t )( void );

bool DetourCreateInterface()
{
	/*
	if( GetLuaShared() == false )
		return false;

	ILuaInterface* pGetThatBitch = NULL;
	do
	{
		while( ( pGetThatBitch = ptr_LuaShared->GetLuaInterface( 2 ) ) == NULL )
			Sleep( 100 );

		DebugMessage( "LUA->Top() = %i", pGetThatBitch->Top() );

		pGetThatBitch->Pop( 2 );

		DebugMessage( "[Global] LUA->Top() = %i", pGetThatBitch->Top() );

		//DebugMessage( "[Global UnRef] LUA->Top() = %i", pGetThatBitch->Top() );

		Sleep( 100 );
	}
	while( true );
	*/

	
	if( GetLuaShared() == false )
		return false;

	OutputDebugStringA( "Making own interface..." );

	ptr_LuaShared->CreateLuaInterface( 3, true );

	ILuaInterface* pHack = ptr_LuaShared->GetLuaInterface( 3 );

	if( pHack )
		OutputDebugStringA( "pHack is valid" );

	SetLuaInterface( pHack );

	OutputDebugStringA( "Calling Init" );

	ILuaCallback* fakeptr = new ILuaCallback;

	pHack->Init( fakeptr, true );

	OutputDebugStringA( "Wow it didn't crash" );

	if( pHack )
	{
		OutputDebugStringA( "Interface exists" );

		pHack->RunString( "test.lua", "", "hellojack = true", true, true );

		OutputDebugStringA( "Interface test..." );

		pHack->Msg( "Hello EveryOne Tyler Here" );

		OutputDebugStringA( "Interface hack..." );
	}

	/*
	OutputDebugStringA( "Hooking CLuaShared::CreateLuaInterface" );

	g_CreateLuaInterface.Hook( ( DWORD )&new_CreateLuaInterface, ( PDWORD )ptr_LuaShared, 4 );

	OutputDebugStringA( "Hooked CLuaShared::CreateLuaInterface" );

	/*
	OutputDebugStringA( "Hooking CLuaShared::CloseLuaInterface" );

	g_CloseLuaInterface.Hook( ( DWORD )&new_CloseLuaInterface, ( PDWORD )ptr_LuaShared, 5 );

	OutputDebugStringA( "Hooked CLuaShared::CloseLuaInterface" );
	*/

	return true;
}

bool DetourRunString_Client()
{
	if( ptr_Lua_Client == NULL )
		return false;

	if( bRSHasHooked_Client == true )
		return false;

	OutputDebugStringA( "Hooking Client CLuaInterface::RunStringEx" );
	
	g_RunStringEx_Client.Hook( ( DWORD )&new_RunStringEx_Client, ( PDWORD )ptr_Lua_Client, 106 );

	bRSHasHooked_Client = true;

	OutputDebugStringA( "Hooked Client CLuaInterface::RunStringEx" );

	return true;
}

bool DetourRunString_Menu()
{
	if( ptr_Lua_Menu == NULL )
		return false;

	if( bRSHasHooked_Menu == true )
		return false;

	OutputDebugStringA( "Hooking Menu CLuaInterface::RunStringEx" );
	
	//g_RunStringEx_Menu.Hook( ( DWORD )&new_RunStringEx_Menu, ( PDWORD )ptr_Lua_Menu, 106 );

	bRSHasHooked_Menu = true;

	OutputDebugStringA( "Hooked Menu CLuaInterface::RunStringEx" );

	return true;
}