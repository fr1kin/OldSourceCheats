// Herpes v4

#ifdef WIN32
 #pragma once
#endif

#include "cluaload.h"

static const char *getS( lua_State *L, void *ud, size_t *size ) 
{
    LoadS *ls = ( LoadS* )ud;

    if ( ls->size == 0 ) 
		return NULL;

    *size = ls->size;
    ls->size = 0;

    return ls->s;
}

void hack::LoadScript( string path )
{
	if( sigs::lua_loadbuffer == NULL )
		return;

	if( g_Interfaces.m_pLua == NULL )
		return;
	
	static LuaLoadBuffer_t lua_loadbuffer = ( LuaLoadBuffer_t )( sigs::lua_loadbuffer );
	
	ifstream flua;
	string full_path = g_Log.FILE_PATH;
	full_path.append( "/lua/" );
	full_path.append( path );

	flua.open( full_path.c_str(), ios::in | ios::binary | ios::ate );
	
	if( !flua.is_open() )
	{
		g_Log.Add( "Failed to load file at %s.", full_path.c_str() );
		return;
	}

	// credits to yakahughes for file shit.

	ifstream::pos_type fsize = flua.tellg();
	char* fcontents = new char[ fsize ];

	flua.seekg( 0, std::ios::beg );
	flua.read( fcontents, fsize );
	flua.close();

	lua_State *L = g_Interfaces.m_pLua->GetLuaState();
	
	LoadS data;
	data.s = fcontents;
	data.size = fsize;

	string name = "[hh]:";
	name.append( path );
	
	g_Interfaces.m_pLua->RunStringEx( name.c_str(), "", fcontents, true, true, true );

	// g_Interfaces.m_pLua->Call( 0, 0 );

	delete[] fcontents;
}