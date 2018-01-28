// Herpes v4
// cinterface is from HerpesCSS

#ifdef WIN32
 #pragma once
#endif

#include "cinterfaces.h"
																						
using namespace std;

CInterfaces		g_Interfaces;
CNetVars		g_NetVars;
CNetVarFinder	g_NetVarFinder;

// Interfaces
IMaterialSystem				*CInterfaces::m_pMaterialSystem;
IClientEntityList			*CInterfaces::m_pClientEntList;
IVEngineClient				*CInterfaces::m_pEngine;
IFileSystem					*CInterfaces::m_pFileSystem;
ICvar						*CInterfaces::m_pCVar;
IBaseClientDLL				*CInterfaces::m_pClient;
IClientDLLSharedAppSystems	*CInterfaces::m_pClientInterfaces;
CPrediction					*CInterfaces::m_pPrediction;
IEngineSound				*CInterfaces::m_pSound;
IGameEventManager2			*CInterfaces::m_pGameEventManager;
IVModelRender				*CInterfaces::m_pModelRender;
IVRenderView				*CInterfaces::m_pRenderView;
IEngineTrace				*CInterfaces::m_pEngineTrace;
IEngineVGui					*CInterfaces::m_pEngineVGui;
IVEfx						*CInterfaces::m_pEffects;
IVModelInfoClient			*CInterfaces::m_pModelInfo;
IVDebugOverlay				*CInterfaces::m_pDebugOverlay;
IStudioRender				*CInterfaces::m_pStudioRender;
IPhysics					*CInterfaces::m_pPhysics;
IPhysicsSurfaceProps		*CInterfaces::m_pPhysicsSurfaceProps;
vgui::ISurface				*CInterfaces::m_pSurface;
vgui::IPanel				*CInterfaces::m_pPanel;
IUniformRandomStream		*CInterfaces::m_pRandom;
CGlobalVarsBase				*CInterfaces::m_pGlobals;
IClientMode					*CInterfaces::m_pClientMode;
CInput						*CInterfaces::m_pInput;
IGameConsole				*CInterfaces::m_pConsole;
vgui::IInput				*CInterfaces::m_pIInput;
ILuaInterface				*CInterfaces::m_pLua;
ILuaShared					*CInterfaces::m_pLuaShared;
IGameMovement				*CInterfaces::m_pGameMovement;

NETVAR_PROP					*CNetVars::m_nTickBase;

ClientClass *GetAllClasses()
{
	ClientClass* pClass = NULL;

	DWORD* pdwClientVMT = ( DWORD* )*( DWORD* )g_Interfaces.m_pClient;
	DWORD dwGetAllClasses = pdwClientVMT[8];

	__asm
	{
		PUSHAD;
		CALL dwGetAllClasses;
		MOV pClass, EAX;
		POPAD;
	}

	return pClass;
}

bool CNetVars::GetAndCheckNetVar( NETVAR_PROP *pProp, const char* czParent, const char* czName )
{
	string strParent	= czParent;
	string strName		= czName;
	
	pProp = g_NetVarFinder.GetPropFromTable( strParent, strName );

	if( pProp )
		return true;
	return false;
}

NETVAR_PROP	*CNetVarFinder::GetPropFromTable( string strTable, string strProp )
{
	NETVAR_CLASS *pTable = this->GetTableInformation( strTable );

	if( !pTable )
		return NULL;

	for( size_t i = 0 ; i < pTable->props.size() ; i++ )
	{
		NETVAR_PROP *pProp = &pTable->props[i];

		if( !pProp )
			continue;

		if( pProp->PropName.compare( strProp ) == 0 )
			return pProp;
	}
	return NULL;
}

bool CNetVars::FindNetVars()
{
	if( g_NetVarFinder.IsReady() == false )
		return false;
	
	CNetVars::m_nTickBase = g_NetVarFinder.GetPropFromTable( "DT_LocalPlayerExclusive", "m_nTickBase" );
	g_Log.Add( "m_nTickBase [0x%X]", ( DWORD )CNetVars::m_nTickBase->Offset );

	return true;
}