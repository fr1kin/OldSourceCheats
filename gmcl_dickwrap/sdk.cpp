#include "sdk.h"

bool CModule::Memory::DataCompare( BYTE* pData, BYTE* bMask, char * szMask )
{
	for( ; *szMask; ++szMask, ++pData, ++bMask )
	{
		if( *szMask == 'x' && *pData != *bMask )
			return false;
	}
	return ( *szMask ) == NULL;
}

DWORD CModule::Memory::FindPattern( DWORD dwAddress, BYTE *bMask, char * szMask )
{
	DWORD dwLen = 0x00FFFFFF;
	for( DWORD i = 0; i < dwLen; i++ )
	{
		if( Memory::DataCompare( (BYTE*)( dwAddress + i ), bMask, szMask ) )
			return ( DWORD )( dwAddress + i );
	}
	return NULL;
}