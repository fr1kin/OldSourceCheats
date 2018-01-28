#include "hooks.h"

using namespace vidja;

typedef toolkit::VMTManager Hook_t;
typedef toolkit::VMTHook VMTHook_t;

Hook_t*		vmtLuaShared;
VMTHook_t*	vmtLuaInterface;

typedef ILuaInterface*( __thiscall* ILuaShared__CreateLuaInterface_t )( void*, unsigned char, bool );
typedef int( __thiscall* ILuaShared__CloseLuaInterface_t )( void*, ILuaInterface* );
typedef bool( __thiscall* ILuaInterface__Init_t )( void*, void*, bool );

void HookLuaInterface( ILuaInterface* pLua )
{
	static bool bHasHooked = false;
	if( bHasHooked == false )
	{
		bHasHooked = true;
		static VMTHook_t LuaInterfaceHook( pLua );
		vmtLuaInterface = &LuaInterfaceHook;
		LuaInterfaceHook.HookMethod( &Hooks::ILuaInterface__Init, ILuaInterface__Init____INDEX );
	}
}

void Hooks::Hook()
{
	if( Interfaces::LuaShared )
	{
#ifdef _vidja_DEBUGMODE
		DebugMessage( "LuaShared pointer valid, hooking..." );
#endif
		static Hook_t LuaSharedHook( Interfaces::LuaShared );
        vmtLuaShared = &LuaSharedHook;

#ifdef _vidja_DEBUGMODE
		DebugMessage( "Hooking ILuaShared::CreateLuaInterface..." );
#endif
		LuaSharedHook.HookMethod( &Hooks::ILuaShared__CreateLuaInterface, ILuaShared__CreateLuaInterface____INDEX );

#ifdef _vidja_DEBUGMODE
		DebugMessage( "Hooking ILuaShared::CloseLuaInterface..." );
#endif
		LuaSharedHook.HookMethod( &Hooks::ILuaShared__CloseLuaInterface, ILuaShared__CloseLuaInterface____INDEX );
	}
}

ILuaInterface* __fastcall Hooks::ILuaShared__CreateLuaInterface( void* thisptr, int edx, unsigned char iState, bool bArg2 )
{
#ifdef _vidja_DEBUGMODE
	DebugMessage( "ILuaShared::CreateLuaInterface called" );
#endif
	
	ILuaInterface* pRet = vmtLuaShared->GetMethod<ILuaShared__CreateLuaInterface_t>( ILuaShared__CreateLuaInterface____INDEX )( thisptr, iState, bArg2 );

#ifdef _vidja_DEBUGMODE
	DebugMessage( "ILuaShared::CreateLuaInterface got return value" );
#endif

	switch( ( int )iState )
	{
	case CLIENT:
		{
			Interfaces::LuaClient = pRet;
			break;
		}
	case SERVER:
		{
			Interfaces::LuaServer = pRet;
			break;
		}
	case MENU:
		{
			Interfaces::LuaMenu = pRet;
			HookLuaInterface( pRet );
			break;
		}
	};

	return pRet;
}

int __fastcall Hooks::ILuaShared__CloseLuaInterface( void* thisptr, int edx, ILuaInterface* pLua )
{
#ifdef _vidja_DEBUGMODE
	DebugMessage( "ILuaShared::CloseLuaInterface called" );
#endif

	int iRet = vmtLuaShared->GetMethod<ILuaShared__CloseLuaInterface_t>( ILuaShared__CloseLuaInterface____INDEX )( thisptr, pLua );

#ifdef _vidja_DEBUGMODE
	DebugMessage( "ILuaShared::CloseLuaInterface got return value" );
#endif

	if( pLua )
	{
		int iState = 0;
		do
		{
			if( ( DWORD )Interfaces::LuaShared->GetLuaInterface( iState ) == ( DWORD )pLua )
				break;

			iState++;
		}
		while( iState <= 2 );

		switch( iState )
		{
		case CLIENT:
			{
				Interfaces::LuaClient = NULL;
				break;
			}
		case SERVER:
			{
				Interfaces::LuaServer = NULL;
				break;
			}
		case MENU:
			{
				DoError( "[ILuaShared__CloseLuaInterface] Menu state is closing, this could be a problem..." );
				Interfaces::LuaMenu = NULL;
				break;
			}
		}
	}

	return iRet;
}

bool __fastcall Hooks::ILuaInterface__Init( void* thisptr, int edx, void* pCallBack, bool bLoadJit )
{
	bool bRet = vmtLuaInterface->GetMethod<ILuaInterface__Init_t>( ILuaInterface__Init____INDEX )( thisptr, pCallBack, bLoadJit );

	ILuaInterface* pLua = ( ILuaInterface* )thisptr;

	if( pLua )
	{
		int iState = 0;
		bool bNoState = false;
		do
		{
			if( ( DWORD )Interfaces::LuaShared->GetLuaInterface( iState ) == ( DWORD )pLua )
				break;

			if( iState == 2 )
				bNoState = true;

			iState++;
		}
		while( iState <= 2 );

		if( bNoState == false )
		{
			switch( iState )
			{
			case CLIENT:
				{
					DebugMessage( "client loaded" );
					LuaClient::AddFuncs();
					Lua::RunAllFilesInFolder( pLua, "client" );
					break;
				}
			case SERVER:
				{
					break;
				}
			case MENU:
				{
					Lua::RunAllFilesInFolder( pLua, "menu" );
					break;
				}
			}
		}
		else
		{
			DoError( "[ILuaInterface__Init] Lua state is invalid" );
		}
	}

	return bRet;
}