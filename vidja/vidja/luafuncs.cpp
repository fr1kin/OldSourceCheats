#include "luafuncs.h"

using namespace vidja;

static const char *getS (lua_State *L, void *ud, size_t *size) 
{
	LoadS *ls = (LoadS*)ud;
	if (ls->size == 0) return NULL;
	*size = ls->size;
	ls->size = 0;
	return ls->s;
}

lua_load_t Lua::LuaLoad = NULL;
std::string Lua::LuaPath = "";

void Lua::FindExports()
{
	if( g_pLuaSharedDll == NULL )
		return;

	DWORD dwLuaLoadPtr = g_pLuaSharedDll->GetProcess( "lua_load" );

	if( dwLuaLoadPtr == NULL )
	{
		DebugMessage( "lua_load ptr couldn't be found!" );
		return;
	}

	Lua::LuaLoad = ( lua_load_t )( dwLuaLoadPtr );
}

void Lua::SetupFolders()
{
	std::string strPath = g_pVidjaDll->GetPath();
	strPath.append( "lua\\" );

	Lua::FormatSlashes( strPath );
	Lua::CreateDirectories( strPath );
	Lua::LuaPath = strPath;
}

void Lua::RunAllFilesInFolder( ILuaInterface* pLua, std::string strFolder )
{
	std::string strFolderPath = Lua::LuaPath;
	strFolderPath.append( strFolder );

	Lua::FormatSlashes( strFolderPath );
	Lua::CreateDirectories( strFolderPath );

	STRVEC Files, Folders;

	Lua::FindLuaFiles( strFolderPath, Files, Folders );

	for( std::size_t i = 0; i < Files.size(); i++ )
	{
		std::string strFilePath = strFolderPath + "\\" + Files[i];
		Lua::LoadFile( pLua, strFilePath );
	}
}

std::streampos Lua::GetFileSize( std::string strFile )
{
	std::ifstream ifFile( strFile, std::ios::in | std::ios::binary | std::ios::ate );

	if( ifFile.is_open() )
	{
		ifFile.seekg( NULL, std::ios::end );
        std::streampos iSize = ifFile.tellg();
        ifFile.seekg( NULL, std::ios::beg );

		ifFile.close();

		return iSize;
	}
	return -1;
}

char* Lua::GetFileContents( std::string strFile, int iSize )
{
	std::ifstream ifFile( strFile, std::ios::in | std::ios::binary | std::ios::ate );

	if( ifFile.is_open() )
	{
		char* chContents = new char[iSize];

        ifFile.seekg( NULL, std::ios::beg );
		ifFile.read( chContents, iSize );

		ifFile.close();

		return chContents;
	}
	return "0";
}

bool Lua::LoadFile( ILuaInterface* pLua, std::string strFile )
{
	if( Lua::LuaLoad == NULL )
		return false;

	std::streampos iSize	= Lua::GetFileSize( strFile );
	char* chContents		= Lua::GetFileContents( strFile, iSize );

	LoadS info;
	info.s = chContents;
	info.size = iSize;

	Lua::LuaLoad( ( lua_State* )pLua->GetLuaState(), ( lua_Reader_t )getS, &info, "xhit" );
	pLua->Call( 0, 0 );

	delete[] chContents;

	return true;
}

void Lua::FormatSlashes( std::string& strPath )
{
	char chPath[MAX_PATH];
	strcpy_s( chPath, strPath.c_str() );
	for( int i = 0; i < strlen( chPath ); i++ )
	{
		if( chPath[i] == '/' )
			chPath[i] = '\\';
	}
	strPath = chPath;
}

void Lua::CreateDirectories( std::string strPath )
{
	Lua::FormatSlashes( strPath );
	std::string strCurrent = "";
	int iPos = NULL, iInfLoopCheck = NULL;
	do
	{
		if( iInfLoopCheck > 500 )
			break;

		iPos = strPath.find( "\\" );

		if( iPos == -1 && strPath != "" )
		{
			strCurrent.append( strPath );
			_mkdir( strCurrent.c_str() );
			break;
		}
		else if( iPos == -1 )
			break;

		std::string strFolder = strPath.substr( 0, iPos );
		strPath = strPath.substr( iPos + 1 );
		strCurrent.append( strFolder + "\\" );
		_mkdir( strCurrent.c_str() );

		iInfLoopCheck++;
	}
	while( true );
}

void Lua::FindLuaFiles( std::string strPath, STRVEC &m_vFiles, STRVEC &m_vFolders )
{
	WIN32_FIND_DATA ffd;
	TCHAR szDir[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;

	FormatSlashes( strPath );

	if( strPath.substr( strPath.length() ) == "\\" )
		strPath.append( "*.lua" );
	else
		strPath.append( "\\*.lua" );

	StringCchCopy( szDir, MAX_PATH, strPath.c_str() );

	hFind = FindFirstFile( szDir, &ffd );

	if ( INVALID_HANDLE_VALUE == hFind )
		return;
	
	do
	{
		if ( ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			m_vFolders.push_back( ffd.cFileName );
		else
			m_vFiles.push_back( ffd.cFileName );
	}
	while( FindNextFile( hFind, &ffd ) != 0 );
	FindClose( hFind );
}