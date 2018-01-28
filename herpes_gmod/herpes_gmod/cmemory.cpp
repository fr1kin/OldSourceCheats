// Herpes v4
// cmemory is from HerpesCSS

#ifdef WIN32
 #pragma once
#endif

#include "cmemory.h"

using namespace std;

CMemory g_Memory;

bool CMemory::DataCompare( const BYTE *pData, const BYTE *bMask, const char *szMask )
{
	for( ; *szMask; ++szMask, ++pData, ++bMask )
	{
		if( *szMask == 'x' && *pData != *bMask )
			return false;
	}
	return (*szMask) == NULL;
}

bool CMemory::TextCompare( const BYTE *pData, const char *czComp )
{
	for ( ; *czComp; ++pData, ++czComp )
		if ( *pData != *czComp )
			return false;
	return true;
}

DWORD CMemory::FindPattern( DWORD dwAddress, BYTE *bMask, char *szMask )
{
	DWORD dwLen = 0x00FFFFFF;
	for( DWORD i = 0; i < dwLen; i++ )
	{
		if( CMemory::DataCompare( (BYTE*)( dwAddress + i ), bMask, szMask ) )
			return ( DWORD )( dwAddress + i );
	}
	return NULL;
}

char *CMemory::FindInterface( char *czInterface, char *czScan )
{
	DWORD dwSizeOfModule = CMemory::GetModuleSize( czScan );
	DWORD dwClient = ( DWORD )GetModuleHandleA( czScan );
	for( DWORD i = 0; i < dwSizeOfModule; i++ )
	{
		if( CMemory::TextCompare( ( PBYTE )( dwClient + i ), czInterface ) )
		{
			return (char*)( dwClient + i );
		}
	}
	return NULL;
}

CreateInterfaceFn CMemory::GetFactory( const char *czInterface )
{
	HMODULE hmModule = GetModuleHandleA( czInterface );

	if( !hmModule )
		return NULL;

	return ( CreateInterfaceFn )GetProcAddress( hmModule, "CreateInterface" );
}

DWORD CMemory::GetHMODULE( const char *czModule )
{
	HMODULE hmModule = GetModuleHandleA( czModule );
	return reinterpret_cast< DWORD >( hmModule );
}

DWORD CMemory::GetModuleSize( char *czModule )
{
	MODULEINFO miInfo;
	HMODULE hmModule = GetModuleHandleA( czModule );

	GetModuleInformation( GetCurrentProcess(), hmModule, &miInfo, sizeof( MODULEINFO ) );
	return miInfo.SizeOfImage;
}

// ph0ne
__declspec( naked, noinline ) PEB_LDR_DATA* GetPEB()
{
	__asm
	{
		MOV EAX, FS:[0x18]
		MOV EAX, [EAX + 0x30]
		MOV EAX, [EAX + 0xC]
		RETN
	}
}

void UNLINK( LIST_ENTRY MyEntry )
{
	MyEntry.Blink->Flink = MyEntry.Flink;
	MyEntry.Flink->Blink = MyEntry.Blink;
}

// Credits to ph0ne for all of this
bool CMemory::CloakModule( HINSTANCE module )
{
	DWORD protect;
	// grab dos pe header

	IMAGE_DOS_HEADER* dos = ( IMAGE_DOS_HEADER* )( module );

	if( !dos || dos->e_magic != IMAGE_DOS_SIGNATURE ) return false;

	// grab nt pe header
	
	IMAGE_NT_HEADERS* nt = ( IMAGE_NT_HEADERS* )( ( BYTE* )module + dos->e_lfanew );

	if( !nt || nt->Signature != IMAGE_NT_SIGNATURE ) return false;

	uintptr_t headersize = nt->OptionalHeader.SizeOfHeaders;

	// remove page protection so we can write
	if( VirtualProtect( dos, headersize, PAGE_READWRITE, &protect ) )
	{
		// clear the PE header

		SecureZeroMemory( dos, headersize );

		// restore page protection

		VirtualProtect( dos, headersize, protect, 0 );

		FlushInstructionCache( GetCurrentProcess(), dos, headersize );
	}

	// grab the head of the linked list of modules

	LDR_MODULE* ldr = ( LDR_MODULE* )( GetPEB()->InLoadOrderModuleList.Flink );

	while( ldr->BaseAddress )
	{
		// stop when we've reached our module

		if( ldr->BaseAddress == module ) break;

		// get the next node
		
		ldr = ( LDR_MODULE* )( ldr->InLoadOrderModuleList.Flink );
	}
	
	// unlink all entries in the list
	
	UNLINK( ldr->HashTableEntry );
	UNLINK( ldr->InLoadOrderModuleList );
	UNLINK( ldr->InMemoryOrderModuleList );
	UNLINK( ldr->InInitializationOrderModuleList );

	// null out the structure
	
	SecureZeroMemory( ldr->FullDllName.Buffer, ldr->FullDllName.Length );
	
	SecureZeroMemory( ldr, sizeof( LDR_MODULE ) );

	return true;
}