// ############################################################
// Herpes V3 for CSS
// File: CUtil.h
// Purpose: Misc functions
// ############################################################

#ifdef WIN32
 #pragma once
#endif

#ifndef __H_CUTIL_
#define __H_CUTIL_

#include "csdk.h"

#define TICKINTERVAL ( g_Interfaces.m_pGlobals->interval_per_tick )

static const float RADTODEG_VALUE = 57.295779513082320876798154814105f;

#define RAD2DEG_ANGLE( x ) ( x * RADTODEG_VALUE )

class CUtil
{
public:
	C_BaseEntity* LocalPlayer();
	float GetCurTime( int future );
	void CompensateSpread( CUserCmd *pCmd, QAngle& qaAngles, Vector &out );
	C_BaseEntity *GetBaseEntity( int iIndex );
	bool GetHitboxPos( int iHitbox, C_BaseEntity *pEnt, Vector& vecOut );
	const char *ToLower( const char *pszString );
	float SqrtR( float flNumber );
};
extern CUtil g_Util;

#endif //__H_CUTIL_