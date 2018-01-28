// Herpes v4

#ifdef WIN32
 #pragma once
#endif

#include "csdk.h"

using namespace hack;

DWORD hack::sigs::lua_load					= NULL;
DWORD hack::sigs::lua_loadbuffer			= NULL;
DWORD hack::sigs::RandomFunc				= NULL;
CreateInterfaceFn hack::sigs::appsysfactory = NULL;

void hack::FingSigs()
{
	if( sigs::appsysfactory == NULL )
	{
		sigs::appsysfactory = **( CreateInterfaceFn** )( g_Memory.FindPattern( 
			g_Memory.GetHMODULE( MODULE_ENGINE ),
			( PBYTE )"\xA1\x00\x00\x00\x00\x8B\x0D\x00\x00\x00\x00\x8B\x11\x83\xC4\x04\x68\x00\x00\x00\x00\x50\x50\x8B\x02",
			"x????xx????xxxxxx????xxxx"
		) + 0x1 );
		
		if( sigs::appsysfactory == NULL )
			g_Log.Add( "Appsysfactory could not be found!" );
		else
			g_Log.Add( "Appsysfactory located at 0x%X", ( DWORD )sigs::appsysfactory );
	}

	if( sigs::RandomFunc == NULL )
	{
		sigs::RandomFunc = g_Memory.FindPattern( 
			g_Memory.GetHMODULE( MODULE_CLIENT ),
			( PBYTE )"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x18\x8D\x0C\x24\xE8\x00\x00\x00\x00\x6A\x01",
			"xxxxxxxxxxxxx????xx"
		);

		if( sigs::RandomFunc == NULL )
			g_Log.Add( "RandomFunc could not be found!" );
		else
			g_Log.Add( "RandomFunc located at 0x%X", ( DWORD )sigs::RandomFunc );
	}

	if( g_Interfaces.m_pGlobals == 0x0 )
	{
		g_Interfaces.m_pGlobals = *reinterpret_cast< CGlobalVarsBase** >( g_Memory.FindPattern( 
			g_Memory.GetHMODULE( MODULE_ENGINE ),
			( PBYTE )"\x68\x00\x00\x00\x00\x50\x50\x8B\x02\xFF\xD0\x85\xC0\x75\x0D",
			"x????xxxxxxxxxx"
		) + 0x01 );

		if( g_Interfaces.m_pGlobals == NULL )
			g_Log.Add( "Globals could not be found!" );
		else
			g_Log.Add( "Globals located at 0x%X", ( DWORD )g_Interfaces.m_pGlobals );
	};
}

void hack::FindInterfaces()
{
	// You can't use appsys for getting IBaseClientDLL.
	Sys_GetFactory(
	CInterfaces::m_pClient				= (IBaseClientDLL*)				g_Memory.GetFactory( "client.dll" )( g_Memory.FindInterface( H_CLIENT, "client.dll" ), NULL );
	CInterfaces::m_pClientEntList		= (IClientEntityList*)			g_Memory.GetFactory( "client.dll" )( g_Memory.FindInterface( H_CLIENTENTITY_LIST, "client.dll" ), NULL );
	CInterfaces::m_pGameMovement		= (IGameMovement*)				g_Memory.GetFactory( "client.dll" )( g_Memory.FindInterface( "GameMovement00", "client.dll" ), NULL );
	CInterfaces::m_pPrediction			= (CPrediction*)				g_Memory.GetFactory( "client.dll" )( g_Memory.FindInterface( "VClientPrediction00", "client.dll" ), NULL );

	CInterfaces::m_pMaterialSystem		= (IMaterialSystem*)			sigs::appsysfactory( g_Memory.FindInterface( H_MATERIAL_SYSTEM, "MaterialSystem.dll" ), NULL );
	CInterfaces::m_pStudioRender		= (IStudioRender*)				sigs::appsysfactory( g_Memory.FindInterface( H_STUDIO_RENDER, "StudioRender.dll" ), NULL );
	CInterfaces::m_pPhysics				= (IPhysics*)					sigs::appsysfactory( g_Memory.FindInterface( H_PHYSICS, "vphysics.dll" ), NULL );
	CInterfaces::m_pPhysicsSurfaceProps	= (IPhysicsSurfaceProps*)		sigs::appsysfactory( g_Memory.FindInterface( H_PHYSICS_SURFACE_PROP, "vphysics.dll" ), NULL );
	CInterfaces::m_pSurface				= (vgui::ISurface*)				sigs::appsysfactory( g_Memory.FindInterface( H_VGUI_SURFACE, "vguimatsurface.dll" ), NULL); 
	CInterfaces::m_pEngine				= (IVEngineClient*)				sigs::appsysfactory( g_Memory.FindInterface( H_ENGINE_CLIENT, "engine.dll" ), NULL );
	CInterfaces::m_pRandom				= (IUniformRandomStream *)		sigs::appsysfactory( g_Memory.FindInterface( H_ENGINE_RANDOM, "engine.dll" ), NULL );
	CInterfaces::m_pSound				= (IEngineSound *)				sigs::appsysfactory( g_Memory.FindInterface( H_ENGINE_SOUND_CLIENT, "engine.dll" ), NULL );
	CInterfaces::m_pGameEventManager	= (IGameEventManager2 *)		sigs::appsysfactory( g_Memory.FindInterface( H_GAMEEVENT_MANAGER, "engine.dll" ), NULL );
	CInterfaces::m_pModelRender			= (IVModelRender *)				sigs::appsysfactory( g_Memory.FindInterface( H_ENGINE_MODEL, "engine.dll" ), NULL );
	CInterfaces::m_pRenderView			= (IVRenderView *)				sigs::appsysfactory( g_Memory.FindInterface( H_ENGINE_RENDERVIEW, "engine.dll" ), NULL );
	CInterfaces::m_pEngineTrace			= (IEngineTrace *)				sigs::appsysfactory( g_Memory.FindInterface( H_ENGINE_TRACECLIENT, "engine.dll" ), NULL );
	CInterfaces::m_pEngineVGui			= (IEngineVGui *)				sigs::appsysfactory( g_Memory.FindInterface( H_ENGINE_VGUI, "engine.dll" ), NULL );
	CInterfaces::m_pEffects				= (IVEfx *)						sigs::appsysfactory( g_Memory.FindInterface( H_ENGINE_EFFECTS, "engine.dll" ), NULL );
	CInterfaces::m_pModelInfo			= (IVModelInfoClient *)			sigs::appsysfactory( g_Memory.FindInterface( H_MODEL_INFOCLIENT, "engine.dll" ), NULL );
	CInterfaces::m_pDebugOverlay		= (IVDebugOverlay *)			sigs::appsysfactory( g_Memory.FindInterface( H_DEBUG_OVERLAY, "engine.dll" ), NULL );
	CInterfaces::m_pClientInterfaces	= (IClientDLLSharedAppSystems*)	sigs::appsysfactory( g_Memory.FindInterface( H_CLIENTDLLSHAREDAPPSYS, "client.dll" ), NULL );
	CInterfaces::m_pPanel				= (vgui::IPanel*)				sigs::appsysfactory( g_Memory.FindInterface( H_VGUI_PANEL, "vgui2.dll" ), NULL );
	CInterfaces::m_pCVar				= (ICvar*)						sigs::appsysfactory( g_Memory.FindInterface( H_ENGINE_CVAR, "vstdlib.dll" ), NULL );
	CInterfaces::m_pIInput				= (vgui::IInput*)				sigs::appsysfactory( g_Memory.FindInterface( "VGUI_Input00", "client.dll" ), NULL );

	DWORD* pdwClientVMT					= ( DWORD* )*( DWORD* )CInterfaces::m_pClient;
	CInterfaces::m_pInput				= ( CInput* )*( DWORD* )*( DWORD* )( pdwClientVMT[21] + 0x28 );

	if( CInterfaces::m_pInput == NULL )
	{
		g_Log.Add( "m_pInput searching..." );

		DWORD dwInput = g_Memory.FindPattern( 
			g_Memory.GetHMODULE( MODULE_CLIENT ),
			( PBYTE )"\x8B\x0D\x00\x00\x00\x00\x8B\x55\x0C\x8B\x01\x8B\x40\x10\x52\x51",
			"xx????xxxxxxxxxx"
		);

		dwInput += 0x2;

		CInterfaces::m_pInput = ( CInput* )*( PDWORD )dwInput;
	}

	if( CInterfaces::m_pInput )
	{
		g_Log.Add( "m_pInput located at 0x%X", ( DWORD )CInterfaces::m_pInput );
	}

	g_NetVarFinder.StoreNetVarInformation( g_Interfaces.m_pClient );
	g_NetVars.FindNetVars();
}

DWORD WINAPI dwMainThread( LPVOID lpArguments )
{
	while( FindWindowA( "Valve001", NULL ) == NULL )
		Sleep( 100 );

	while( GetModuleHandleA( MODULE_ENGINE ) == NULL || GetModuleHandleA( MODULE_CLIENT ) == NULL || GetModuleHandleA( MODULE_SHARED ) == NULL )
		Sleep( 100 );

	g_Log.Add( "##############################" );
	g_Log.Add( "] New game started" );
	g_Log.Add( "##############################" );

	hack::FingSigs();
	hack::FindInterfaces();
	hack::HookFuncs();

	ConCommand *cmd = CInterfaces::m_pCVar->FindCommand( "lua_run_cl" );
	
	return 0;
}

BOOL WINAPI DllMain( HINSTANCE hInstance, DWORD dwReasonOfCall, LPVOID lpReserved )
{
	if ( dwReasonOfCall == DLL_PROCESS_ATTACH )
	{
		DisableThreadLibraryCalls( ( HMODULE )hInstance );

		g_Log.GetFilePath( ( HMODULE )hInstance );
		g_Log.Create( "gmod.log" );

		g_Memory.CloakModule( ( HMODULE )hInstance );

		CreateThread( NULL, 0, ( LPTHREAD_START_ROUTINE )dwMainThread, NULL, 0, NULL );
	}

	return TRUE;
}