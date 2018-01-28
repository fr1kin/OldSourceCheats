// ############################################################
// Herpes V3 for CSS
// File: CUtil.cpp
// Purpose: Misc functions
// ############################################################

#ifdef WIN32
 #pragma once
#endif

#include "CUtil.h"

CUtil g_Util;

C_BaseEntity* CUtil::LocalPlayer()
{
	if( g_Interfaces.m_pGlobals == NULL )
		return NULL;

	if( g_Interfaces.m_pClientEntList == NULL )
		return NULL;

	IClientEntity *pClientEnt = g_Interfaces.m_pClientEntList->GetClientEntity( g_Interfaces.m_pEngine->GetLocalPlayer() );

	if( !pClientEnt || pClientEnt == NULL )
		return NULL;

	C_BaseEntity *pLocal = pClientEnt->GetBaseEntity();

	if( !pLocal )
	{
		return NULL;
	}

	return pLocal;
}

float CUtil::GetCurTime( int future )
{
	C_BaseEntity *pLocal = g_Util.LocalPlayer();

	if( !pLocal )
		return g_Interfaces.m_pGlobals->curtime;

	DWORD dwEnt = reinterpret_cast< DWORD >( g_Util.LocalPlayer() );
	DWORD dwOffset = g_NetVars.m_nTickBase->Offset;

	int TickBase = *reinterpret_cast< int* >( dwEnt + dwOffset );

	return ( float )( TickBase + future ) * g_Interfaces.m_pGlobals->interval_per_tick;
}

C_BaseEntity *CUtil::GetBaseEntity( int iIndex )
{
	assert( iIndex );

	if( g_Interfaces.m_pClientEntList == NULL )
		return NULL;

	IClientEntity *pClientEnt = g_Interfaces.m_pClientEntList->GetClientEntity( iIndex );

	if( !pClientEnt || pClientEnt == NULL )
		return NULL;

	return pClientEnt->GetBaseEntity();
}

bool CUtil::GetHitboxPos( int iHitbox, C_BaseEntity *pEnt, Vector& vecOut )
{
	assert( pEnt );

	matrix3x4_t mMatrix[ MAXSTUDIOBONES ];

	const model_t *pModel = pEnt->GetModel();

	if( pModel == NULL )
		return false;

	studiohdr_t *pStudioModel = g_Interfaces.m_pModelInfo->GetStudiomodel( pModel );

	if( pStudioModel == NULL )
		return false;

	bool bSetup = pEnt->SetupBones( mMatrix, 128, BONE_USED_BY_HITBOX, g_Interfaces.m_pGlobals->curtime );

	if( bSetup == false )
		return false;

	mstudiohitboxset_t *pHitboxSet = pStudioModel->pHitboxSet( 0 );

	if( pHitboxSet == NULL )
		return false;

	mstudiobbox_t* pStudioBox = pStudioModel->pHitbox( iHitbox, 0 );

	if( pStudioBox == NULL )
		return false;

	Vector vecMin, vecMax;

	VectorTransform( pStudioBox->bbmin, mMatrix[ pStudioBox->bone ], vecMin );
	VectorTransform( pStudioBox->bbmax, mMatrix[ pStudioBox->bone ], vecMax );

	vecOut = ( vecMin + vecMax ) * 0.5f;
	return true;
}

void CUtil::CompensateSpread( CUserCmd *pCmd, QAngle& qaAngles, Vector &out )
{
	uint uiSeed = pCmd->random_seed & 255;
	uiSeed++;

	RandomSeed( uiSeed );

	Vector vecRandom( 0, 0, 0 );

	do 
	{
		vecRandom.x = RandomFloat( -1, 1 ) * 0.5 + RandomFloat( -1, 1 ) * 0.5;
		vecRandom.y = RandomFloat( -1, 1 ) * 0.5 + RandomFloat( -1, 1 ) * 0.5;
		vecRandom.z = vecRandom.x * vecRandom.x + vecRandom.y * vecRandom.y;
	}
	while( vecRandom.z > 1 );

	Vector vecForward( 0, 0, 0 );
	Vector vecRight( 0, 0, 0 );
	Vector vecUp( 0, 0, 0 );

	AngleVectors( qaAngles, &vecForward, &vecRight, &vecUp );

	out = vecForward + vecRandom.x * 0.7 * vecRight + vecRandom.y * 0.7 * vecUp;
	out.NormalizeInPlace();
}

const char *CUtil::ToLower( const char *pszString )
{
	assert( pszString );

	char chBuffer[150];
	sprintf( chBuffer, "%s", pszString );

	for( size_t i = 0; i <= strlen( chBuffer ); i++ )
	{
		chBuffer[i] = tolower( chBuffer[i] );
	}

	const char *pszRet = chBuffer;
	return pszRet;
}

float CUtil::SqrtR( float flNumber )
{
	const float f = 1.5F;

	float x = flNumber * 0.5F;
	float y = flNumber;

	long i = *( long* )( &y );

	i = 0x5F3759DF - ( i >> 1 );

	y = *( float* )( &i );
	y = y * ( f - ( x * y * y ) );

	return y;
}