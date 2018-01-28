// Herpes v4

#ifdef WIN32
 #pragma once
#endif

#ifndef ____csdk__
#define ____csdk__

#define WIN32_LEAN_AND_MEAN
#define CLIENT_DLL
#define GAME_DLL

#pragma comment( lib, "tier0.lib" )
#pragma comment( lib, "tier1.lib" )
#pragma comment( lib, "tier2.lib" )
#pragma comment( lib, "tier3.lib" )
#pragma comment( lib, "mathlib.lib" )
#pragma comment( lib, "vstdlib.lib" )
#pragma comment( lib, "vgui_controls.lib" )

#pragma warning( disable : 4311 )
#pragma warning( disable : 4312 )
#pragma warning( disable : 4541 )
#pragma warning( disable : 4267 )
#pragma warning( disable : 4183 )

#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <sstream>
#include <math.h>
#include <Psapi.h>
#include <time.h>
#include <Winbase.h>
#include <time.h>

#include "ILuaShared.h"
#include "ILuaObject.h"

#include "cluainterface.h"

#include "ADE32.h"
#include "detourxs.h"

#include "VMTHooks.h"

#include <cdll_int.h>
#include <filesystem.h>
#include <cdll_client_int.h>
#include <mathlib\mathlib.h>
#include <iclientmode.h>
#include <iefx.h>
#include <engine\IEngineSound.h>
#include <ienginevgui.h>
#include <engine\ivdebugoverlay.h>
#include <vgui\ISurface.h>
#include <iprediction.h>
#include <igameevents.h>
#include <vgui\IVGui.h>
#include <input.h>
#include <con_nprint.h>
#include <game_controls\commandmenu.h>
#include <in_buttons.h>
#include <vphysics_interface.h>
#include <ivrenderview.h>
#include <vgui/ilocalize.h>
#include <engine\IEngineTrace.h>
#include <convar.h>
#include <icvar.h>
#include <tier1.h>
#include <eiface.h>
#include <math.h>
#include <usercmd.h>
#include <checksum_md5.h>
#include <vstdlib/random.h>
#include <mathlib/vector.h>
#include <icliententity.h>
#include <icliententitylist.h>
#include <materialsystem/imaterial.h>
#include <materialsystem/imaterialsystem.h>
#include <materialsystem/imaterialvar.h>
#include <inetchannelinfo.h>
#include <cdll_int.h>
#include <cdll_client_int.h>
#include <tier1/tier1.h>
#include <tier2/tier2.h>
#include <tier3/tier3.h>
#include <interface.h>
#include <mathlib\mathlib.h>
#include <cbase.h>
#include <common/GameUI/IGameConsole.h>
#include "prediction.h"
#include "igamemovement.h"

#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

namespace hack
{
	class sigs
	{
	public:
		static DWORD lua_load;
		static DWORD lua_loadbuffer;
		static DWORD RandomFunc;
		static CreateInterfaceFn appsysfactory;
	};
	
	void Hook();
	void FingSigs();
	void FindInterfaces();
};

#include "clog.h"
#include "cmemory.h"

#include "hook.h"
#include "cutil.h"

#include "cinterfaces.h"
#include "cgetinterface.h"
#include "cluaload.h"
#include "cluafuncs.h"

#endif //____csdk__