// Herpes v4

#ifdef WIN32
 #pragma once
#endif

#ifndef ____chook__
#define ____chook__

class CVerifiedUserCmd
{
	public:
		CUserCmd	m_cmd;
		CRC32_t		m_crc;
};

typedef void ( __stdcall *RunScriptsInFolder_t )( const char*, void* unknown );
typedef void ( __stdcall *RandomFunc_t )( void );

#define MULTIPLAYER_BACKUP_GMOD 128

namespace hack
{
	namespace hooks
	{
		void __stdcall RunScriptsInFolder( const char* pszFile, void* unknown );
		void __stdcall RandomFunc( void );
		
		CUserCmd* __fastcall GetUserCmd( void* thisptr, int edx, int sequence_num );
		bool __stdcall WriteUsercmdDeltaToBuffer( bf_write *buf, int from, int to, bool isnewcommand );
		void __fastcall CreateMove( void* thisptr, int edx, int iSequenceNumber, float flInputSampleFrameTime, bool bActive );

		class mem
		{
		public:
			static DWORD dwRunScriptsInFolder;
			static DWORD dwRandomFunc;
		};
	}

	void HookFuncs();
}

#endif //____chook__