#ifndef __vidja_luafuncs_
#define __vidja_luafuncs_

#include "sdk.h"

typedef std::vector<std::string> STRVEC;

namespace vidja
{
	namespace Lua
	{
		void FindExports();
		void SetupFolders();
		bool LoadFile( ILuaInterface* pLua, std::string strFile );
		std::streampos GetFileSize( std::string strFile );
		char* GetFileContents( std::string strFile, int iSize );
		void FormatSlashes( std::string& strPath );
		void CreateDirectories( std::string strPath );
		void RunAllFilesInFolder( ILuaInterface* pLua, std::string strFolder );
		void FindLuaFiles( std::string strPath, STRVEC &m_vFiles, STRVEC &m_vFolders );
		extern lua_load_t LuaLoad;
		extern std::string LuaPath;
	};
};

#endif