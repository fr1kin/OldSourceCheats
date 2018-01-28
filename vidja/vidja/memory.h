class CMemory;
#ifndef __vidja_memory_
#define __vidja_memory_

#include "sdk.h"

#define HCLIENTDLL			"client.dll"
#define HENGINEDLL			"engine.dll"
#define HLUASHAREDDLL		"lua_shared.dll"
#define HSERVERDLL			"server.dll"

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

typedef void* ( __cdecl* CreateInterface_t )( const char*, int* );

class CMemory
{
public:
	CMemory( HMODULE hmMod );
	CMemory( DWORD dwMod );
	bool AssertF();
	DWORD FindPattern( const char* czMask, const char* _czMask );
	void* GetInterface( const char* pszName );
	DWORD GetProcess( const char* pszProcessName );
	const char* GetMappedName();
	const char* GetPath();
private:
	CreateInterface_t GetFactory();
	bool DataCompare( const BYTE *pData, const BYTE *bMask, const char *szMask );
	HMODULE m_hmMod;
	CreateInterface_t m_fnInterface;
	bool m_bModuleNULL;
	char m_chPath[228];
};

namespace vidja
{
	namespace Memory
	{
		template< typename T > inline T GetCallAddress( DWORD dwCall )
		{
			return ( T )( *( PDWORD )( dwCall + 0x1 ) + dwCall + 0x5 );
		};
		CMemory* GetModule( const char* pszModName );
		void GetModule( CMemory** pMem, const char* pszModName );
		void GetModule( CMemory** pMem, DWORD dwModName );
		bool ModuleLoaded( const char* pszModName );
	};
};

#endif