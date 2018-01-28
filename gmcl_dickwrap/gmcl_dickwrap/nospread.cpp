#include "cnospread.h"

using namespace GarrysMod::Lua;

int CModule::Predict( lua_State* L )
{
	if( g_pLua == NULL )
		return 0;

	g_pLua->CheckType( 1, Type::USERCMD );
	g_pLua->CheckType( 2, Type::VECTOR );
	g_pLua->CheckType( 3, Type::VECTOR );

	CUserCmd* pCmd = ( CUserCmd* )( g_pLua->GetUserData( 1 ) );

	if( pCmd == NULL )
		return 0;

	unsigned int CmdSeed = *( ( int* )pCmd + 1 );

	pCmd->random_seed = MD5_PseudoRandom( pCmd->command_number ) & 0x7fffffff;

	unsigned int Seed = pCmd->random_seed;

	static int ConstantSeed = 0;

	if( CmdSeed != NULL )
		ConstantSeed = Seed;

	RandomSeed( ConstantSeed & 255 );

	Vector Forward = *( Vector* )( g_pLua->GetUserData( 2 ) );
	Vector Right, Up;

	VectorVectors( Forward, Right, Up );

	Vector &Spread = *( Vector* )( g_pLua->GetUserData( 3 ) );

	float x, y, z;

	do
	{
		x = RandomFloat( -1, 1 ) * 0.5 + RandomFloat( -1, 1 ) * 0.5;
		y = RandomFloat( -1, 1 ) * 0.5 + RandomFloat( -1, 1 ) * 0.5;
		z = x * x + y * y;
	} while( z > 1 );

	Vector Result( 0, 0, 0 );

	Result = Forward + x * Spread.x * Right + y * Spread.y * Up;

	if( Result == NULL )
		return 0;

	ILuaObject* VectorObject = g_pLua->GetGlobal( "Vector" );

	if( VectorObject == NULL )
		return 0;

	g_pLua->Push( VectorObject );
	g_pLua->Push( Result.x );
	g_pLua->Push( Result.y );
	g_pLua->Push( Result.z );
	g_pLua->Call( 3, 1 );

	ILuaObject* Return = g_pLua->GetReturn( 0 );

	if( Return )
		g_pLua->Push( Return );
	else
		g_pLua->PushNil();

	VectorObject->UnReference();
	Return->UnReference();

	return 1;
}