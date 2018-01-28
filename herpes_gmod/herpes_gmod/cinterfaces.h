// Herpes v4
// cinterface is from HerpesCSS

#ifdef WIN32
 #pragma once
#endif

#ifndef __H_CINTERFACES_
#define __H_CINTERFACES_

#include "csdk.h"

using namespace std;

#define __ADD_LOG( x, ... ) g_Log.Add( x, ... )

typedef struct {
	RecvProp*				Prop;
	string					PropName;
	int						Offset;
} NETVAR_PROP;

typedef struct {
	string					ClassName;
	string					TableName;
	vector<NETVAR_PROP>	props;
} NETVAR_CLASS;

extern ClientClass *GetAllClasses();

class CInterfaces
{
public:
	static IMaterialSystem				*m_pMaterialSystem;
	static IClientEntityList			*m_pClientEntList;
	static IVEngineClient				*m_pEngine;
	static IFileSystem					*m_pFileSystem;
	static ICvar						*m_pCVar;
	static IBaseClientDLL				*m_pClient;
	static IClientDLLSharedAppSystems	*m_pClientInterfaces;
	static CPrediction					*m_pPrediction;
	static IEngineSound					*m_pSound;
	static IGameEventManager2			*m_pGameEventManager;
	static IVModelRender				*m_pModelRender;
	static IVRenderView					*m_pRenderView;
	static IEngineTrace					*m_pEngineTrace;
	static IEngineVGui					*m_pEngineVGui;
	static IVEfx						*m_pEffects;
	static IVModelInfoClient			*m_pModelInfo;
	static IVDebugOverlay				*m_pDebugOverlay;
	static IStudioRender				*m_pStudioRender;
	static IPhysics						*m_pPhysics;
	static IPhysicsSurfaceProps			*m_pPhysicsSurfaceProps;
	static vgui::ISurface				*m_pSurface;
	static vgui::IPanel					*m_pPanel;
	static IUniformRandomStream			*m_pRandom;
	static CGlobalVarsBase				*m_pGlobals;
	static IClientMode					*m_pClientMode;
	static CInput						*m_pInput;
	static IGameConsole					*m_pConsole;
	static INetChannelInfo				*m_pNet();
	static vgui::IInput					*m_pIInput;
	static ILuaInterface				*m_pLua;
	static ILuaShared					*m_pLuaShared;
	static IGameMovement				*m_pGameMovement;
};
extern CInterfaces g_Interfaces;

class CNetVars
{
public:
	bool GetAndCheckNetVar( NETVAR_PROP *pVar, const char* czParent, const char* czName );
	bool FindNetVars( void );

	static NETVAR_PROP	*m_nTickBase;
};
extern CNetVars g_NetVars;

class CNetVarFinder
{
public:
	CNetVarFinder()
	{
		m_vNetVars.clear();
	}
	
	bool IsReady()
	{
		return ( m_vNetVars.size() > 0 );
	}

	void DumpTable( ClientClass *pClass, RecvTable *pTable )
	{
		if( pTable->GetNumProps() <= 0 )
			return;
		
		if( !Q_strstr( pTable->GetName(), "DT_" ) )
			return;

		NETVAR_CLASS nvClass;

		nvClass.ClassName = pClass->GetName();
		nvClass.TableName = pTable->GetName();

		for( int i = 0 ; i < pTable->GetNumProps() ; i++ )
		{
			RecvProp *pProp = pTable->GetProp( i );

			if( !pProp )
				continue;

			RecvTable *pChildTable = pProp->GetDataTable();

			if( pChildTable && ( pChildTable->GetNumProps() > 0 ) )
				DumpTable( pClass, pChildTable );

			NETVAR_PROP nvProp;
			
			nvProp.PropName	= pProp->GetName();
			nvProp.Offset	= pProp->GetOffset();
			nvProp.Prop		= pProp;

			nvClass.props.push_back( nvProp );
		}
		m_vNetVars.push_back( nvClass );
		return;
	}

	bool StoreNetVarInformation( IBaseClientDLL* pClient )
	{
		if( pClient == NULL )
			return false;

		m_vNetVars.clear();

		ClientClass *pClass = GetAllClasses();

		for( ; pClass ; pClass = pClass->m_pNext )
		{
			RecvTable *pTable = pClass->m_pRecvTable;
			DumpTable( pClass, pTable );
		}
		return IsReady();
	}

	NETVAR_CLASS *GetTableInformation( string strTable )
	{
		for( size_t i = 0 ; i < m_vNetVars.size() ; i++ )
		{
			if( m_vNetVars[i].TableName.compare( strTable ) == 0 )
			{
				return &m_vNetVars[i];
			}
		}
		return NULL;
	}

	NETVAR_CLASS *GetClassInformation( string strName )
	{
		for( size_t i = 0 ; i < m_vNetVars.size(); i++ )
		{
			if( m_vNetVars[i].ClassName.compare( strName ) == 0 )
			{
				return &m_vNetVars[i];
			}
		}
		return NULL;
	}
	
	NETVAR_PROP	*GetPropFromTable( string strTable, string strProp );

	NETVAR_PROP	*GetPropFromClass( string strName, string strProp )
	{
		NETVAR_CLASS *pClass = GetClassInformation( strName );

		if( !pClass )
			return NULL;

		for( size_t i = 0 ; i < pClass->props.size() ; i++ )
		{
			NETVAR_PROP *pProp = &pClass->props[i];

			if( !pProp )
				continue;

			if( pProp->PropName.compare( strProp ) == 0 )
			{
				return pProp;
			}
		}
		return NULL;
	}

	RecvVarProxyFn HookClassProp( string strName, string strProp, RecvVarProxyFn pNewProp )
	{
		NETVAR_PROP *pProp = GetPropFromClass( strName, strProp );

		if( !pProp )
			return NULL;

		RecvVarProxyFn store = pProp->Prop->GetProxyFn();

		pProp->Prop->SetProxyFn( pNewProp );

		return store;
	}
public:
	vector< NETVAR_CLASS > m_vNetVars;
};
extern CNetVarFinder g_NetVarFinder;

#endif //__H_CINTERFACES_