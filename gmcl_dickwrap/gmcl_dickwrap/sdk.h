#pragma once

#ifndef __sdk______
#define __sdk______

#define GAME_DLL
#define CLIENT_DLL

#include <windows.h>
#include <string>

#include "gmheaders\Interface.h"
#include "gmheaders\ILuaInterface.h"
#include "gmheaders\ILuaModuleManager.h"

#include <cdll_int.h>
#include <cdll_client_int.h>
#include <mathlib\mathlib.h>
#include <iefx.h>
#include <input.h>
#include <con_nprint.h>
#include <in_buttons.h>
#include <tier1.h>
#include <eiface.h>
#include <math.h>
#include <usercmd.h>
#include <checksum_md5.h>
#include <vstdlib/random.h>
#include <mathlib/vector.h>
#include <icliententity.h>
#include <icliententitylist.h>
#include <tier1/tier1.h>
#include <tier2/tier2.h>
#include <tier3/tier3.h>
#include <interface.h>
#include <mathlib\mathlib.h>
#include <cbase.h>
#include <decals.h>
//#include <game\client\c_baseanimating.h>

namespace CModule
{
	namespace Memory
	{
		bool DataCompare( BYTE* pData, BYTE* bMask, char * szMask );
		DWORD FindPattern( DWORD dwAddress, BYTE *bMask, char * szMask );
	}
}

#include "main.h"

#endif //__sdk______