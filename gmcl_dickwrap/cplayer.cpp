#include "player.h"

using namespace GarrysMod::Lua;

IClientEntityList *ptr_EntList = NULL;

bool CModule::GetClientEntList()
{
	if( ptr_EntList != NULL )
		return true;

	CreateInterfaceFn clientFactory = ( CreateInterfaceFn )GetProcAddress( GetModuleHandleA( "client.dll" ), "CreateInterface" );

	if( clientFactory == NULL )
		return false;

	ptr_EntList = ( IClientEntityList* )clientFactory( VCLIENTENTITYLIST_INTERFACE_VERSION, NULL );

	if( ptr_EntList == NULL )
		return false;

	return true;
}

int CModule::IsDormant( lua_State* L )
{
	if( ptr_EntList == NULL )
		return NULL;

	if( g_pLua == NULL )
		return NULL;

	g_pLua->CheckType( 1, Type::NUMBER );

	int iEntIndex = g_pLua->GetNumber( 1 );

	IClientEntity* pEnt = ptr_EntList->GetClientEntity( iEntIndex );

	if( pEnt )
	{
		g_pLua->Push( pEnt->IsDormant() );
	}
	else
		g_pLua->PushNil();

	return 1;
}

int CModule::DrawHitbox( lua_State* L )
{
	if( ptr_EntList == NULL )
		return NULL;

	if( g_pLua == NULL )
		return NULL;

	g_pLua->CheckType( 1, Type::NUMBER );
	g_pLua->CheckType( 2, Type::NUMBER );

	int iEntIndex = g_pLua->GetNumber( 1 );
	float flDur = g_pLua->GetNumber( 2 );

	IClientEntity* pEnt = ptr_EntList->GetClientEntity( iEntIndex );

	if( pEnt )
	{
		CBaseEntity* pEntity = pEnt->GetBaseEntity();

		C_BaseAnimating* pAni = pEntity->GetBaseAnimating();

		if( pAni == NULL )
			return 0;

		pAni->DrawClientHitboxes( flDur, false );
	}

	return 0;
}