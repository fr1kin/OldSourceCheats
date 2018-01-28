#include "memory.h"

using namespace vidja;

CMemory* Memory::GetModule( const char* pszModName )
{
	if( !Memory::ModuleLoaded( pszModName ) )
	{
		DoError( "[Memory::GetModule( const char* pszModName )] Module %s is NULL", pszModName );
		return NULL;
	}
	return new CMemory( GetModuleHandleA( pszModName ) );
}

void Memory::GetModule( CMemory** pMem, const char* pszModName )
{
	if( !Memory::ModuleLoaded( pszModName ) )
	{
		DoError( "[Memory::GetModule( CMemory* pMem, const char* pszModName )] Module %s is NULL", pszModName );
		return;
	}
	*pMem = new CMemory( GetModuleHandleA( pszModName ) );
}

void Memory::GetModule( CMemory** pMem, DWORD dwModName )
{
	if( dwModName == NULL )
	{
		DoError( "[Memory::GetModule( CMemory* pMem, DWORD dwModName )] Module is NULL" );
		return;
	}
	*pMem = new CMemory( ( HMODULE )dwModName );
}

bool Memory::ModuleLoaded( const char* pszModName )
{
	if( GetModuleHandleA( pszModName ) == NULL )
		return false;
	return true;
}

CMemory::CMemory( HMODULE hmMod )
{
	m_hmMod = hmMod;
	m_bModuleNULL = hmMod == NULL ? true : false;
	m_fnInterface = ( CreateInterface_t )GetFactory();
	GetPath();
}

CMemory::CMemory( DWORD dwMod )
{
	m_hmMod = ( HMODULE )dwMod;
}

bool CMemory::AssertF()
{
	if( m_bModuleNULL )
	{
		DoError( "[CMemory::Assert()] m_dwMod is NULL" );
		return false;
	}
	return true;
}

DWORD CMemory::FindPattern( const char* czMask, const char* _czMask )
{
	assert( AssertF() );
	DWORD dwLen = 0x00FFFFFF;
	PBYTE bMask = ( PBYTE )( czMask );
	PCHAR szMask = (PCHAR )( _czMask );
	for( DWORD i = 0; i < dwLen; i++ )
	{
		if( DataCompare( (BYTE*)( m_hmMod + i ), bMask, szMask ) )
			return ( DWORD )( m_hmMod + i );
	}
	return NULL;
}

void* CMemory::GetInterface( const char* pszName )
{
	assert( AssertF() );
	void* pInterface = NULL;
	CreateInterface_t fnInterface = ( CreateInterface_t )GetProcAddress( m_hmMod, "CreateInterface" );
	for( int i = 0; i < 100; ++i )
	{
		char chInterfaceName[128];
		sprintf_s( chInterfaceName, ( i < 10 ) ? "%s00%d" : "%s0%d", pszName, i );
		pInterface = ( fnInterface )( chInterfaceName, NULL );
		if( pInterface )
			break;
	}
	return pInterface;
}

DWORD CMemory::GetProcess( const char* pszProcessName )
{
	assert( AssertF() );
	return ( DWORD )GetProcAddress( ( HMODULE )m_hmMod, pszProcessName );
}

const char* CMemory::GetMappedName()
{
	MEMORY_BASIC_INFORMATION info;
	for( DWORD dwAdd = 0x1; dwAdd < 0x7FFFFFFF; dwAdd += info.RegionSize )
	{
		if( VirtualQuery( ( PVOID )( dwAdd ), &info, sizeof( info ) ) == NULL )
			return NULL;

		char czBuff[MAX_PATH];

		if( GetMappedFileName( GetCurrentProcess(), ( PVOID )( dwAdd ), czBuff, MAX_PATH ) == NULL )
			continue;

		if( ( DWORD )( info.AllocationBase ) == ( DWORD )( &__ImageBase ) )
			return czBuff;
	}
	return NULL;
}

const char* CMemory::GetPath()
{
	assert( AssertF() );

	static bool bHasPath = false;

	if( bHasPath )
		return ( const char* )m_chPath;
	
	if( GetModuleFileNameA( ( HMODULE )m_hmMod, m_chPath, MAX_PATH ) == NULL )
		return NULL;

	for( int i = ( int )( strlen( m_chPath ) ); i > 0; i-- )
	{
		if( m_chPath[i] == '\\' )
		{
			m_chPath[i + 1] = 0;
			break;
		}
	}

	bHasPath = true;

	return m_chPath;
}

bool CMemory::DataCompare( const BYTE *pData, const BYTE *bMask, const char *szMask )
{
	assert( AssertF() );
	for( ; *szMask; ++szMask, ++pData, ++bMask )
	{
		if( *szMask == 'x' && *pData != *bMask )
			return false;
	}
	return (*szMask) == NULL;
}

CreateInterface_t CMemory::GetFactory()
{
	assert( AssertF() );
	if( m_hmMod == NULL )
		return NULL;
	return ( CreateInterface_t )GetProcess( "CreateInterface" );
}