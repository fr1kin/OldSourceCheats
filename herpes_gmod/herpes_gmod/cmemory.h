// Herpes v4
// cmemory is from HerpesCSS

#ifdef WIN32
 #pragma once
#endif

#ifndef __H_CMEMORY_
#define __H_CMEMORY_

#include "csdk.h"

using namespace std;

#define MODULE_CLIENT	"client.dll"	
#define MODULE_ENGINE	"engine.dll"
#define MODULE_SHARED	"lua_shared.dll"

#define H_MATERIAL_SYSTEM			"VMaterialSystem0"
#define H_STUDIO_RENDER				"VStudioRender0"
#define H_PHYSICS					"VPhysics0"
#define H_PHYSICS_SURFACE_PROP		"VPhysicsSurfaceProps00"
#define H_VGUI_SURFACE				"VGUI_Surface0"
#define H_ENGINE_CLIENT				"VEngineClient0"
#define H_ENGINE_RANDOM				"VEngineRandom00"
#define H_ENGINE_SOUND_CLIENT		"IEngineSoundClient00"
#define H_GAMEEVENT_MANAGER			"GAMEEVENTSMANAGER00"
#define H_ENGINE_MODEL				"VEngineModel0"
#define H_ENGINE_RENDERVIEW			"VEngineRenderView0"
#define H_ENGINE_TRACECLIENT		"EngineTraceClient00"
#define H_ENGINE_VGUI				"VEngineVGui00"
#define H_ENGINE_EFFECTS			"VEngineEffects00"
#define H_MODEL_INFOCLIENT			"VModelInfoClient00"
#define H_DEBUG_OVERLAY				"VDebugOverlay00"
#define H_CLIENTENTITY_LIST			"VClientEntityList00"
#define H_CLIENTDLLSHAREDAPPSYS		"VClientDllSharedAppSystems00"
#define H_CLIENTPREDICTION			"VClientPrediction00"
#define H_CLIENT					"VClient0"
#define H_VGUI_PANEL				"VGUI_Panel00"
#define H_ENGINE_CVAR				"VEngineCvar00"
#define H_STEAM_CLIENT				"SteamClient0"
#define H_STEAM_FRIENDS				"SteamFriends00"
#define H_STEAM_FRIENDS				"SteamFriends00"
#define H_GAME_CONSOLE				"GameConsole00"

typedef struct _LSA_UNICODE_STRING {
  USHORT Length;
  USHORT MaximumLength;
  PWSTR  Buffer;
} LSA_UNICODE_STRING, *PLSA_UNICODE_STRING, UNICODE_STRING, *PUNICODE_STRING;

typedef struct _LDR_MODULE {
	  LIST_ENTRY              InLoadOrderModuleList;
	  LIST_ENTRY              InMemoryOrderModuleList;
	  LIST_ENTRY              InInitializationOrderModuleList;
	  PVOID                   BaseAddress;
	  PVOID                   EntryPoint;
	  ULONG                   SizeOfImage;
	  UNICODE_STRING          FullDllName;
	  UNICODE_STRING          BaseDllName;
	  ULONG                   Flags;
	  SHORT                   LoadCount;
	  SHORT                   TlsIndex;
	  LIST_ENTRY              HashTableEntry;
	  ULONG                   TimeDateStamp;
} LDR_MODULE, *PLDR_MODULE;

typedef struct _PEB_LDR_DATA {
	ULONG Length;
	BOOLEAN Initialized;
	PVOID SsHandle;
	LIST_ENTRY InLoadOrderModuleList;
	LIST_ENTRY InMemoryOrderModuleList;
	LIST_ENTRY InInitializationOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef struct _PEB {
	BYTE Reserved1[2];
	BYTE BeingDebugged;
	BYTE Reserved2[1];
	PVOID Reserved3[2];
	PPEB_LDR_DATA Ldr;
} PEB, *PPEB;

inline void**& GetVirtualTable( void* pVoid, size_t iOffset = 0 )
{
    return *reinterpret_cast< void*** >( ( size_t )pVoid + iOffset );
}

inline const void** GetVirtualTable( const void* pVoid, size_t iOffset = 0 )
{
    return *reinterpret_cast< const void*** >( ( size_t )pVoid + iOffset );
}

class CMemory
{
public:
	DWORD GetModuleSize( char *czModule );
	DWORD GetHMODULE( const char *czModule );
	char *FindInterface( char *czInterface, char *czScan );
	CreateInterfaceFn GetFactory( const char *czInterface );
	DWORD FindPattern( DWORD dwAddress, BYTE *bMask, char *szMask );
	template< typename Fn >
	inline Fn GetMethod( const void* pVoid, size_t iIndex, size_t iOffset = 0 )
	{
		return reinterpret_cast< Fn >( GetVirtualTable( pVoid, iOffset )[ iIndex ] );
	}
	bool CloakModule( HMODULE module );
private:
	bool TextCompare( const BYTE *pData, const char *czComp );
	bool DataCompare( const BYTE *pData, const BYTE *bMask, const char *szMask );
};
extern CMemory g_Memory;

#endif //__H_CMEMORY_