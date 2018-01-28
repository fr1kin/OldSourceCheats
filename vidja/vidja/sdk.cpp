#include "sdk.h"

using namespace std;
using namespace vidja;

CMemory* g_pClientDll					= NULL;
CMemory* g_pEngineDll					= NULL;
CMemory* g_pLuaSharedDll				= NULL;
CMemory* g_pVidjaDll					= NULL;

CInput*			Interfaces::Input		= NULL;
ILuaShared*		Interfaces::LuaShared	= NULL;
ILuaInterface*	Interfaces::LuaClient	= NULL;
ILuaInterface*	Interfaces::LuaServer	= NULL;
ILuaInterface*	Interfaces::LuaMenu		= NULL;

const char* vidja::m_pszGetLuaPath		= NULL;

void vidja::DoError( const char* pszMsg, ... )
{
	char chFileName[264];
	sprintf_s( chFileName, "%s%s", g_pVidjaDll->GetPath(), "error.log" );

	char chBuffer[512];
	ofstream ofFile;
	va_list args;

	ofFile.open( chFileName, ios::app );

	va_start( args, pszMsg );
		_vsnprintf_s( chBuffer, sizeof( chBuffer ), pszMsg, args );
	va_end( args );

	OutputDebugStringA( chBuffer );

	ofFile << chBuffer << endl;

	ofFile.close();
}

void vidja::DebugMessage( const char* pszMsg, ... )
{
#ifdef _vidja_DEBUGMODE
	char buff[258];
	va_list args;
	va_start( args, pszMsg );
		_vsnprintf_s( buff, sizeof( buff ), pszMsg, args );
	va_end( args );
	OutputDebugStringA( buff );
#endif
}

#ifdef _vidja_DEBUGMODE
bool vidja::DebugCheckPointers()
{
	if( vidja::Interfaces::LuaClient == NULL )
		DebugMessage( "LuaClient PTR is NULL!" );
	if( vidja::Interfaces::LuaServer == NULL )
		DebugMessage( "LuaServer PTR is NULL!" );
	if( vidja::Interfaces::LuaMenu == NULL )
		DebugMessage( "LuaMenu PTR is NULL!" );
	if( vidja::Interfaces::LuaShared == NULL )
		DebugMessage( "LuaShared PTR is NULL!" );
	if( vidja::Lua::LuaLoad == NULL )
		DebugMessage( "LuaLoad PTR is NULL!" );

	DebugMessage( "DLL Path: %s", g_pVidjaDll->GetPath() );

	return true;
}
#endif