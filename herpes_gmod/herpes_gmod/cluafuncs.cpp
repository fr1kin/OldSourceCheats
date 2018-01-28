// Herpes v4

#ifdef WIN32
 #pragma once
#endif

#include "csdk.h"

using namespace hack;

bool funcs::vars::prospread		= true;
bool funcs::vars::prediction	= true;
int  funcs::vars::seed			= 225;

// Include - Runs a lua file WITHIN the directory.
LUA_FUNCTION( Include )
{
	if( g_Interfaces.m_pLua == NULL )
		CInterfaces::m_pLua = interfaces::GetInterface( 0 );

	g_Interfaces.m_pLua->CheckType( 1, GLua::TYPE_STRING );

	hack::LoadScript( g_Interfaces.m_pLua->GetString( 1 ) );

	return 0;
}

// Log - Message writing.
LUA_FUNCTION( Log )
{
	if( g_Interfaces.m_pLua == NULL )
		CInterfaces::m_pLua = interfaces::GetInterface( 0 );

	g_Interfaces.m_pLua->CheckType( 1, GLua::TYPE_STRING );

	ConColorMsg( Color( 255, 255, 0, 255 ), g_Interfaces.m_pLua->GetString( 1 ) );

	return 0;
}

// CurTime - Faster, more updated curtime.
LUA_FUNCTION( CurTime )
{
	if( g_Interfaces.m_pLua == NULL )
		CInterfaces::m_pLua = interfaces::GetInterface( 0 );

	g_Interfaces.m_pLua->PushValue( g_Util.GetCurTime( 0 ) );
	return 1;
}

// SetVar - SetVar.
LUA_FUNCTION( SetVar )
{
	if( g_Interfaces.m_pLua == NULL )
		CInterfaces::m_pLua = interfaces::GetInterface( 0 );

	g_Interfaces.m_pLua->CheckType( 1, GLua::TYPE_STRING );
	g_Interfaces.m_pLua->CheckType( 2, GLua::TYPE_NUMBER );
	
	string strVar = g_Interfaces.m_pLua->GetString( 1 );
	int iValue = g_Interfaces.m_pLua->GetNumber( 2 );

	// Crappy right now
	if( strVar == "nospread" )
		funcs::vars::prospread = iValue == 1 ? true : false;
	else if( strVar == "prediction" )
		funcs::vars::prospread = iValue == 1 ? true : false;
	else if( strVar == "seed" )
		funcs::vars::seed = iValue;

	return 0;
}

LUA_FUNCTION( IsDormant )
{
	if( g_Interfaces.m_pLua == NULL )
		CInterfaces::m_pLua = interfaces::GetInterface( 0 );

	g_Interfaces.m_pLua->CheckType( 1, GLua::TYPE_NUMBER );

	
	int index	= g_Interfaces.m_pLua->GetNumber( 1 );

	C_BaseEntity *pEnt = g_Util.GetBaseEntity( index );

	if( !pEnt )
		return NULL;

	g_Interfaces.m_pLua->Push( pEnt->IsDormant() );

	return 1;
}

LUA_FUNCTION( GetHitbox )
{
	if( g_Interfaces.m_pLua == NULL )
		CInterfaces::m_pLua = interfaces::GetInterface( 0 );

	g_Interfaces.m_pLua->CheckType( 1, GLua::TYPE_NUMBER );
	g_Interfaces.m_pLua->CheckType( 2, GLua::TYPE_NUMBER );
	
	int index	= g_Interfaces.m_pLua->GetNumber( 1 );
	int hitbox	= g_Interfaces.m_pLua->GetNumber( 2 );

	C_BaseEntity *pEnt = g_Util.GetBaseEntity( index );

	if( !pEnt )
		return NULL;

	Vector vecHitbox( NULL, NULL, NULL );
	
	if( g_Util.GetHitboxPos( hitbox, pEnt, vecHitbox ) == false )
		return NULL;

	ILuaObject* pLib = g_Interfaces.m_pLua->GetGlobal( "Vector" );
	g_Interfaces.m_pLua->Push( pLib );
	g_Interfaces.m_pLua->Push( ( float )vecHitbox.x );
	g_Interfaces.m_pLua->Push( ( float )vecHitbox.y );
	g_Interfaces.m_pLua->Push( ( float )vecHitbox.z );
	g_Interfaces.m_pLua->Call( 3, 1 );

	ILuaObject* pRet = g_Interfaces.m_pLua->GetReturn( 0 );

	if ( pRet )
		g_Interfaces.m_pLua->Push( pRet );
	else
		g_Interfaces.m_pLua->PushNil();

	pLib->UnReference();
	pRet->UnReference();

	return 1;
}

LUA_FUNCTION( EstimateAbsVelocity )
{
	if( g_Interfaces.m_pLua == NULL )
		CInterfaces::m_pLua = interfaces::GetInterface( 0 );

	g_Interfaces.m_pLua->CheckType( 1, GLua::TYPE_NUMBER );

	int iIndex = g_Interfaces.m_pLua->GetNumber( 1 );

	C_BaseEntity *pEnt = g_Util.GetBaseEntity( iIndex );

	if( !pEnt )
		return NULL;

	Vector vecOut( NULL, NULL, NULL );

	// static EstimateAbsVelocity_t EstimateAbsVelocity = ( EstimateAbsVelocity_t )( sigs::EstimateAbsVelocity );

	// EstimateAbsVelocity( pEnt, vecOut );

	ILuaObject* pLib = g_Interfaces.m_pLua->GetGlobal( "Vector" );
	g_Interfaces.m_pLua->Push( pLib );
	g_Interfaces.m_pLua->Push( ( float )vecOut.x );
	g_Interfaces.m_pLua->Push( ( float )vecOut.y );
	g_Interfaces.m_pLua->Push( ( float )vecOut.z );
	g_Interfaces.m_pLua->Call( 3, 1 );

	ILuaObject* pRet = g_Interfaces.m_pLua->GetReturn( 0 );

	if ( pRet )
		g_Interfaces.m_pLua->Push( pRet );
	else
		g_Interfaces.m_pLua->PushNil();

	pLib->UnReference();
	pRet->UnReference();

	return 1;
}

LUA_FUNCTION( CompensateSpread )
{
	if( g_Interfaces.m_pLua == NULL )
		CInterfaces::m_pLua = interfaces::GetInterface( 0 );

	g_Interfaces.m_pLua->CheckType( 1, GLua::TYPE_USERCMD );
	g_Interfaces.m_pLua->CheckType( 2, GLua::TYPE_VECTOR );
	g_Interfaces.m_pLua->CheckType( 3, GLua::TYPE_VECTOR );

	CUserCmd* pCmd = ( CUserCmd* )( g_Interfaces.m_pLua->GetUserData( 1 ) );

	if( pCmd == NULL )
		return NULL;

	size_t seed = pCmd->random_seed & 255;

	RandomSeed( seed );

	Vector vecForward	= *( Vector* )( g_Interfaces.m_pLua->GetUserData( 2 ) );
	Vector vecRight		= NULL;
	Vector vecUp		= NULL;

	VectorVectors( vecForward, vecRight, vecUp );

	Vector &vecSpread = *( Vector* )( g_Interfaces.m_pLua->GetUserData( 3 ) );

	Vector vecRandom(
			RandomFloat( -1, 1 ) * 0.5 + RandomFloat( -1, 1 ) * 0.5,
			RandomFloat( -1, 1 ) * 0.5 + RandomFloat( -1, 1 ) * 0.5,
			0.0f
	);

	Vector vecResult( NULL, NULL, NULL );
	vecResult = vecForward + vecRandom.x * vecSpread.x * vecRight + vecRandom.y * vecSpread.y * vecUp;

	ILuaObject* pLib = g_Interfaces.m_pLua->GetGlobal( "Vector" );
	g_Interfaces.m_pLua->Push( pLib );
	g_Interfaces.m_pLua->Push( ( float )vecResult.x );
	g_Interfaces.m_pLua->Push( ( float )vecResult.y );
	g_Interfaces.m_pLua->Push( ( float )vecResult.z );
	g_Interfaces.m_pLua->Call( 3, 1 );

	ILuaObject* pRet = g_Interfaces.m_pLua->GetReturn( 0 );

	if ( pRet )
		g_Interfaces.m_pLua->Push( pRet );
	else
		g_Interfaces.m_pLua->PushNil();

	pLib->UnReference();
	pRet->UnReference();

	return 1;
}

LUA_FUNCTION( GetLatency )
{
	if( g_Interfaces.m_pLua == NULL )
		CInterfaces::m_pLua = interfaces::GetInterface( 0 );

	g_Interfaces.m_pLua->CheckType( 1, GLua::TYPE_NUMBER );

	int iChannel = g_Interfaces.m_pLua->GetNumber( 1 );

	INetChannelInfo *pNet = g_Interfaces.m_pEngine->GetNetChannelInfo();

	if( pNet == NULL )
		return NULL;

	g_Interfaces.m_pLua->Push( pNet->GetLatency( iChannel ) );

	return 1;
}

LUA_FUNCTION( GetAvgLatency )
{
	if( g_Interfaces.m_pLua == NULL )
		CInterfaces::m_pLua = interfaces::GetInterface( 0 );

	g_Interfaces.m_pLua->CheckType( 1, GLua::TYPE_NUMBER );

	int iChannel = g_Interfaces.m_pLua->GetNumber( 1 );

	INetChannelInfo *pNet = g_Interfaces.m_pEngine->GetNetChannelInfo();

	if( pNet == NULL )
		return NULL;

	g_Interfaces.m_pLua->Push( pNet->GetAvgLatency( iChannel ) );

	return 1;
}

// Kill - Kill the global table.
LUA_FUNCTION( Kill )
{
	if( g_Interfaces.m_pLua == NULL )
		CInterfaces::m_pLua = interfaces::GetInterface( 0 );

	ILuaObject* obj = g_Interfaces.m_pLua->GetGlobal( "InitializeTable" );
	
	if( obj->isNil() == false )
	{
		g_Interfaces.m_pLua->SetGlobal( "InitializeTable", "" );
		g_Interfaces.m_pLua->RunString( "", "", "if( InitializeTable ) then InitializeTable = nil end", false, true );
	}

	obj->UnReference();
	
	return 0;
}

// InitializeTable - Table for EVERYTHING
LUA_FUNCTION( InitializeTable )
{
	if( g_Interfaces.m_pLua == NULL )
		CInterfaces::m_pLua = interfaces::GetInterface( 0 );

	ILuaObject* tbl = g_Interfaces.m_pLua->GetNewTable();
	
	tbl->SetMember( "Include", Include );
	tbl->SetMember( "Log", Log );
	tbl->SetMember( "CurTime", CurTime );
	tbl->SetMember( "IsDormant", IsDormant );
	tbl->SetMember( "GetHitbox", GetHitbox );
	tbl->SetMember( "CompensateSpread", CompensateSpread );
	tbl->SetMember( "GetLatency", GetLatency );
	tbl->SetMember( "GetAvgLatency", GetAvgLatency );
	tbl->SetMember( "EstimateAbsVelocity", EstimateAbsVelocity );
	tbl->SetMember( "SetVar", SetVar );
	tbl->SetMember( "Kill", Kill );

	tbl->SetMember( "path", g_Log.FILE_PATH );

	g_Interfaces.m_pLua->Push( tbl );

	tbl->UnReference();

	return 1;
}

void hack::funcs::AddFuncs()
{
	if( g_Interfaces.m_pLua == NULL )
		CInterfaces::m_pLua = interfaces::GetInterface( 0 );
	
	g_Interfaces.m_pLua->SetGlobal( "InitializeTable", InitializeTable );
}