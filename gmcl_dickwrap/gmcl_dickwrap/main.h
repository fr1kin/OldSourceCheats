#pragma once

#ifndef __main______
#define __main______

#include "sdk.h"

extern ILuaInterface* g_pLua;

namespace CModule
{
	namespace Main
	{
		void Open();
		void Close();
	};
};

#endif //__main______