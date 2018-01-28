// Herpes v4
// From Herpes CSS

#ifdef WIN32
 #pragma once
#endif

#include "CLog.h"

using namespace std;

#define SHOULD_LOG

CLog g_Log;

void CLog::Create( const char *czFileName )
{
	sprintf( this->FILE_NAME, "%s%s", this->FILE_PATH, czFileName );

	ofstream ofFile;
		ofFile.open( this->FILE_NAME, ios::app );
	ofFile.close();
}

void CLog::Add( PCHAR czTest, ... )
{
#ifdef SHOULD_LOG
	char chBuffer[512];
	ofstream ofFile;
	va_list args;

	ofFile.open( this->FILE_NAME, ios::app );

	va_start( args, czTest );
		_vsnprintf( chBuffer, sizeof( chBuffer ), czTest, args );
	va_end( args );

	ofFile << chBuffer << endl;

	ofFile.close();
#endif
}

// Credits to s0beit
void CLog::GetFilePath( HMODULE hmModule )
{
	memset( this->FILE_PATH, 0, MAX_PATH );
	
	if( GetModuleFileNameA( hmModule, this->FILE_PATH, MAX_PATH ) != 0 )
	{
		for( int i = ( int )( strlen( this->FILE_PATH ) ); i > 0; i-- )
		{
			if( this->FILE_PATH[i] == '\\' )
			{
				this->FILE_PATH[i + 1] = 0;
				break;
			}
		}
	}
}