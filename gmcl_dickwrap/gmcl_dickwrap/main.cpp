#include "main.h"

#include "cnospread.h"
#include "cplayer.h"

ILuaInterface* g_pLua = NULL;

GMOD_MODULE( GM_OPEN, GM_CLOSE );

int GM_OPEN( lua_State* L )
{
	g_pLua = Lua();

	ILuaObject* Table = g_pLua->GetNewTable();

	Table->SetMember( "Predict", CModule::Predict );
	Table->SetMember( "IsDormant", CModule::IsDormant );
	Table->SetMember( "DrawHitbox", CModule::DrawHitbox );

	g_pLua->SetGlobal( "dickwrap", Table );

	Table->UnReference();

	Msg( "\ndickwrap loaded\n" );

	return 0;
}

int GM_CLOSE( lua_State* L )
{
	return 0;
}