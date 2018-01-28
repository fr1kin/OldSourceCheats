#ifndef __VAR_HEADER__
#define __VAR_HEADER__

#include "SDK.h"

namespace NNetworkedVariables
{
	namespace DT_CSPlayer
	{
		extern netvar_prop_t*		m_angEyeAngle;
	};
};

#define NETWORKED_VARIABLE_MACRO( C, V ) NNetworkedVariables::C::V = GNetworkedVariables.GetPropFromTable( #C, #V );

bool Var_Initialize( void );
bool StoreDynamicVariables( void );

#endif