#include "clientfuncs.h"
#include "checksummd5.h"

using namespace vidja;

// TODO:
// Nospread
// Cvar shit
// Name stealing shit
// Anti-screenshot

// ADD a global func
void SetGlobal( const char* pszName, CFunc fn )
{
	if( Interfaces::LuaClient == NULL )
		return;

	Interfaces::LuaClient->PushSpecial( 0 );
		Interfaces::LuaClient->PushString( pszName, 0 );
		Interfaces::LuaClient->PushCFunction( fn );
		Interfaces::LuaClient->SetTable( -3 );
	Interfaces::LuaClient->Pop( 1 );
}

// Lua funcs below here

int LUAFUNC_LoadLua( lua_State* L )
{
	if( Interfaces::LuaClient == NULL )
		return 0;

	Interfaces::LuaClient->CheckType( 1, LuaTypes::TYPE_STRING );

	std::string strPath = Lua::LuaPath;
	strPath.append( Interfaces::LuaClient->GetString( 1, 0 ) );

	Lua::LoadFile( Interfaces::LuaClient, strPath );

	return 0;
}

/*
int LUAFUNC_PredictSpread( lua_State* L )
{
	if( Interfaces::LuaClient == NULL )
		return;

	Interfaces::LuaClient->CheckType( 1, LuaTypes::TYPE_USERCMD );
	Interfaces::LuaClient->CheckType( 2, LuaTypes::TYPE_VECTOR );
	Interfaces::LuaClient->CheckType( 3, LuaTypes::TYPE_VECTOR );

	CGMUserCmd* pCmd = ( CGMUserCmd* )( Interfaces::LuaClient->GetUserdata( 1 ) );

	if( pCmd == NULL )
		return 0;

	pCmd->random_seed = MD5_PseudoRandom( pCmd->command_number ) & 0x7fffffff;

	RandomSeed( pCmd->random_seed & 255 );

	float flForward[3];//*( Vector* )( g_pLua->GetUserData( 2 ) );;
	float flRight[3];
	float flUp[3];

	float flSpread[3]; //*( Vector* )( g_pLua->GetUserData( 3 ) );
	float flPos[3];

	flPos[0] = RandomFloat( -1, 1 ) * 0.5 + RandomFloat( -1, 1 ) * 0.5;
	flPos[1] = RandomFloat( -1, 1 ) * 0.5 + RandomFloat( -1, 1 ) * 0.5;
	flPos[2] = flPos[0] * flPos[0] + flPos[1] * flPos[1];

	float flResult[3];
	
	// Result = Forward + x * Spread.x * Right + y * Spread.y * Up;

	float flX = flForward[0] + flPos[0] * flSpread[0];

	flResult[0] = flX * flRight[0];
	flResult[1];
	flResult[2];

	// flResult = flForward + flHitPos[0] * flSpread[0] * flRight[0] + flHitPos[1] * flSpread[2] * flUp;

	Interfaces::LuaClient->PushSpecial( 0 );
		Interfaces::LuaClient->GetField( -1, "Vector" );
		ILuaObject* o = new ILuaObject( m_pLua, m_pLua->ReferenceCreate() );
	Interfaces::LuaClient->Pop();

	ILuaObject* pVector = ( ILuaObject* )Interfaces::LuaClient->Global( "Vector" );

	if( pVector == NULL )
		return 0;

	Interfaces::LuaClient->Push( pVector );
	Interfaces::LuaClient->Push( Result.x );
	Interfaces::LuaClient->Push( Result.y );
	Interfaces::LuaClient->Push( Result.z );
	Interfaces::LuaClient->Call( 3, 1 );

	ILuaObject* Return = Interfaces::LuaClient->GetReturn( 0 );

	if( Return )
		Interfaces::LuaClient->Push( Return );
	else
		Interfaces::LuaClient->PushNil();

	VectorObject->UnReference();
	Return->UnReference();

	return 1;
}
*/

// add these functions
void LuaClient::AddFuncs()
{
	SetGlobal( "LUA_LoadFile", LUAFUNC_LoadLua );
}