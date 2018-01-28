// Herpes v4

#ifdef WIN32
 #pragma once
#endif

#include "csdk.h"

using namespace hack;

CVMTManager *g_InputHook = NULL;
CVMTManager *g_ClientHook = NULL;

DWORD hooks::mem::dwRunScriptsInFolder = NULL;
DWORD hooks::mem::dwRandomFunc = NULL;

DWORD temp = NULL;

void __fastcall hooks::CreateMove( void* thisptr, int edx, int iSequenceNumber, float flInputSampleFrameTime, bool bActive )
{
	g_ClientHook->GetMethod< void ( __thiscall* )( void*, int, float, bool ) >( 21 )( thisptr, iSequenceNumber, flInputSampleFrameTime, bActive );

	if( !g_Interfaces.m_pInput )
		return;

	g_Log.Add( "1" );

	CUserCmd *pCmd = g_Interfaces.m_pInput->GetUserCmd( iSequenceNumber );

	g_Log.Add( "2" );

	if( !pCmd )
		return;

	g_Log.Add( "3" );

	CVerifiedUserCmd *pVerified = ( CVerifiedUserCmd* )( *( DWORD* )( ( DWORD )g_Interfaces.m_pInput + 0xB8 ) + ( ( 1 << 7 ) + 4 ) * ( iSequenceNumber % 90 ) );

	g_Log.Add( "4" );

	if( !pVerified )
		return;
	
	/*
	C_BasePlayer *pPlayer = static_cast< C_BasePlayer* >( g_Util.LocalPlayer() );
	
	if( funcs::vars::prediction == true && pPlayer )
	{
		float fl_Frame		= g_Interfaces.m_pGlobals->frametime;
		float fl_Curtime	= g_Interfaces.m_pGlobals->curtime;
			
		g_Interfaces.m_pGlobals->frametime	= g_Interfaces.m_pGlobals->interval_per_tick;
		g_Interfaces.m_pGlobals->curtime	= g_Util.GetCurTime( 1 );

		*( CUserCmd** )( ( DWORD )pPlayer + 0x14C0 ) = pCmd;

		char move[200];

		g_Log.Add( "5" );

		g_Interfaces.m_pPrediction->SetupMove( pPlayer, pCmd, 0, ( CMoveData* )&move );
		g_Interfaces.m_pGameMovement->ProcessMovement( pPlayer, ( CMoveData* )&move );
		g_Interfaces.m_pPrediction->FinishMove( pPlayer, pCmd, ( CMoveData* )&move );

		g_Log.Add( "62" );
		
		g_Interfaces.m_pGlobals->frametime	= fl_Frame;
		g_Interfaces.m_pGlobals->curtime	= fl_Curtime;
		
		*( CUserCmd** )( ( DWORD )pPlayer + 0x14C0 ) = 0;
	}
	*/

	if( funcs::vars::prospread == true )
	{
		int iSeed = iSequenceNumber;

		for( ; ( ( MD5_PseudoRandom( iSeed ) & 0x7FFFFFFF ) & 0xFF ) != 33 ; iSeed++ );
			
		pCmd->command_number	= iSeed;
		pCmd->random_seed		= MD5_PseudoRandom( pCmd->command_number ) & 0x7FFFFFFF;
	}

	if( g_Interfaces.m_pLua != NULL )
	{
		ILuaObject* pHook = g_Interfaces.m_pLua->GetGlobal( "hook" );
		if( pHook && pHook->isNil() == false )
		{
			ILuaObject* pCall = pHook->GetMember( "Call" );
			if( pCall && pCall->isNil() == false )
			{
				ILuaObject *pUserCmdInfo = g_Interfaces.m_pLua->GetMetaTable( "CUserCmd", GLua::TYPE_USERCMD );

				if( pUserCmdInfo && pUserCmdInfo->isNil() == false )
				{
					g_Interfaces.m_pLua->Push( pCall );
						g_Interfaces.m_pLua->Push( "CHLClientCreateMove" );
						g_Interfaces.m_pLua->PushNil( );
						g_Interfaces.m_pLua->PushUserData( pUserCmdInfo, pCmd );
					g_Interfaces.m_pLua->Call( 3, 1 );

					pUserCmdInfo->UnReference();
				}
				pCall->UnReference();
			}
			pHook->UnReference();
		}
	}

	g_Log.Add( "7" );

	pVerified->m_cmd = *pCmd;
	pVerified->m_crc = pCmd->GetChecksum();

	g_Log.Add( "8" );
}

CUserCmd* __fastcall hooks::GetUserCmd( void* thisptr, int edx, int sequence_num )
{
	return ( CUserCmd* )( *( PDWORD )( reinterpret_cast< DWORD >( thisptr ) + 0xB4 ) + ( sequence_num % 90 << 7 ) );
}

void __stdcall hooks::RunScriptsInFolder( const char* pszFile, void* unknown )
{
	static RunScriptsInFolder_t CLua__RunScriptsInFolder = ( RunScriptsInFolder_t )( hooks::mem::dwRunScriptsInFolder );
	
	const char* file;
	const char* whatisthis;

	__asm
	{
		mov file, edi
		mov whatisthis, esi
	}

	g_Log.Add( "%s : %s", file, whatisthis );

	if( file )
	{
		ILuaInterface *pLuaInterface = interfaces::GetInterface( 0 );

		if( pLuaInterface )
			CInterfaces::m_pLua = pLuaInterface;

		if( file == "includes/enum" )
		{
			g_Log.Add( "[Herpes] Running lua/..." );

			//hack::funcs::AddFuncs();

			//hack::LoadScript( "init.lua" );
		}

		if( file == "vgui" )
			CInterfaces::m_pLua = NULL;
	}

	CLua__RunScriptsInFolder( pszFile, unknown );
}

void __stdcall hooks::RandomFunc( )
{
	static RandomFunc_t __RandomFunc = ( RandomFunc_t )( hooks::mem::dwRandomFunc );
	
	ILuaInterface *pLuaInterface = interfaces::GetInterface( 0 );

	if( pLuaInterface )
		CInterfaces::m_pLua = pLuaInterface;

	g_Log.Add( "[Herpes] Running lua/..." );

	// hack::funcs::AddFuncs();

	// hack::LoadScript( "init.lua" );

	__RandomFunc( );
}

void hack::HookFuncs()
{
	static CVMTManager ClientHook( g_Interfaces.m_pClient, true );
	g_ClientHook = &ClientHook;

	ClientHook.HookMethod( &hooks::CreateMove, 21 );

	static CVMTManager InputHook( g_Interfaces.m_pInput, true );
	g_InputHook = &InputHook;
	
	InputHook.HookMethod( &hooks::GetUserCmd, 8 );

	// hooks::mem::dwRunScriptsInFolder = ( DWORD )DetourCreate( ( PVOID )sigs::RunScriptInFolder, hooks::RunScriptsInFolder, DETOUR_TYPE_JMP );

	// hooks::mem::dwRandomFunc = ( DWORD )DetourCreate( ( PVOID )sigs::RandomFunc, hooks::RandomFunc, DETOUR_TYPE_JMP );
}