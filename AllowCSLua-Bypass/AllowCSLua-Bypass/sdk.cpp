#include "sdk.h"

char dllPath[100];

bool DataCompare( const BYTE *pData, const BYTE *bMask, const char *szMask )
{
	for( ; *szMask; ++szMask, ++pData, ++bMask )
	{
		if( *szMask == 'x' && *pData != *bMask )
			return false;
	}
	return (*szMask) == NULL;
}

DWORD FindPattern( DWORD dwAddress, const char* czMask, const char* _czMask )
{
	DWORD dwLen = 0x00FFFFFF;
	PBYTE bMask = ( PBYTE )( czMask );
	PCHAR szMask = (PCHAR )( _czMask );
	for( DWORD i = 0; i < dwLen; i++ )
	{
		if( DataCompare( (BYTE*)( dwAddress + i ), bMask, szMask ) )
			return ( DWORD )( dwAddress + i );
	}
	return NULL;
}

void DebugMessage( const char* pszMsg, ... )
{
	char buff[258];
	va_list args;
	va_start( args, pszMsg );
		_vsnprintf( buff, sizeof( buff ), pszMsg, args );
	va_end( args );
	OutputDebugStringA( buff );
}