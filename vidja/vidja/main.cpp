#include "main.h"

using namespace vidja;

DWORD __stdcall vidja::OnInject( LPVOID lpArguments )
{
	while( !Memory::ModuleLoaded( HCLIENTDLL ) || !Memory::ModuleLoaded( HENGINEDLL ) || !Memory::ModuleLoaded( HLUASHAREDDLL ) )
		Sleep( 10 );

#ifdef _vidja_DEBUGMODE
	DebugMessage( "Injected" );
	DebugMessage( "Getting modules..." );
#endif
	
	Memory::GetModule( &g_pClientDll, HCLIENTDLL );
	Memory::GetModule( &g_pEngineDll, HENGINEDLL );
	Memory::GetModule( &g_pLuaSharedDll, HLUASHAREDDLL );

#ifdef _vidja_DEBUGMODE
	DebugMessage( "Getting LuaShared pointer..." );
#endif
	
	Interfaces::LuaShared	= ( ILuaShared* )( g_pLuaSharedDll->GetInterface( "LUASHARED" ) );

#ifdef _vidja_DEBUGMODE
	DebugMessage( "Getting lua_load pointer..." );
#endif

	Lua::FindExports();

#ifdef _vidja_DEBUGMODE
	DebugMessage( "Hooking functions..." );
#endif

	Hooks::Hook();
	Lua::SetupFolders();

#ifdef _vidja_DEBUGMODE
	DebugMessage( "Pointer checking..." );
#endif

	vidja::DebugCheckPointers();
	
	return NULL;
}

int __stdcall DllMain( HINSTANCE hInstance, DWORD dwReasonOfCall, LPVOID lpReserved )
{
	switch( dwReasonOfCall )
	{
	case DLL_PROCESS_ATTACH:
		{
			Memory::GetModule( &g_pVidjaDll, ( DWORD )hInstance );
			DisableThreadLibraryCalls( ( HMODULE )hInstance );
			CreateThread( NULL, NULL, ( LPTHREAD_START_ROUTINE )vidja::OnInject, NULL, NULL, NULL );
		}
	}
	return 1;
}