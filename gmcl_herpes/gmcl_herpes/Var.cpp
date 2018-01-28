#include "SDK.h"

//#define ___LOG_OFFSETS__

netvar_prop_t* NNetworkedVariables::DT_CSPlayer::m_angEyeAngle					= 0;

bool Var_Initialize( void )
{
	return GNetworkedVariables.StoreNetworkedVariableInfo( HERPES::Interfaces::m_pClient );
}

bool StoreDynamicVariables( void )
{
	if( GNetworkedVariables.IsReady() == false )
	{
		return false;
	}
	
	NNetworkedVariables::DT_CSPlayer::m_angEyeAngle	= GNetworkedVariables.GetPropFromTable( "DT_CSPlayer", "m_angEyeAngles[0]" );
	
	char buffer[100];
	sprintf( buffer, "0x%X", NNetworkedVariables::DT_CSPlayer::m_angEyeAngle->offset );

	HERPES::DebugMessage( buffer );

	return true;
}