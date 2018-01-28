// Herpes v4

#ifdef WIN32
 #pragma once
#endif

#ifndef ____cluafuncs__
#define ____cluafuncs__

#include "csdk.h"

#define LUA_FUNCTION( _function_ ) static int _function_( lua_State* L )

typedef void ( __thiscall* EstimateAbsVelocity_t )( C_BaseEntity* thisptr, Vector& vec );

namespace hack
{
	namespace funcs
	{
		void AddFuncs();

		class vars
		{
		public:
			static bool prospread;
			static int seed;

			static bool prediction;
		};
	}
};

#endif //____cluafuncs__