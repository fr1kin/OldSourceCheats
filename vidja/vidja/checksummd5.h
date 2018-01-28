//========= Copyright � 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Generic MD5 hashing algo
//
//=============================================================================//

#ifndef CHECKSUM_MD5_H
#define CHECKSUM_MD5_H

#include "sdk.h"

#ifdef _WIN32
#pragma once
#endif
/*
extern "C" __declspec( dllimport ) void Q_binarytohex( const unsigned char *in, int inputbytes, char *out, int outsize );

extern "C" __declspec( dllimport ) void		RandomSeed( int iSeed );
extern "C" __declspec( dllimport ) float	RandomFloat( float flMinVal = 0.0f, float flMaxVal = 1.0f );

// 16 bytes == 128 bit digest
#define MD5_DIGEST_LENGTH 16  

// MD5 Hash
typedef struct
{
	unsigned int	buf[4];
    unsigned int	bits[2];
    unsigned char	in[64];
} MD5Context_t;

void MD5Init( MD5Context_t *context );
void MD5Update( MD5Context_t *context, unsigned char const *buf, unsigned int len );
void MD5Final( unsigned char digest[ MD5_DIGEST_LENGTH ], MD5Context_t *context );

char *MD5_Print(unsigned char *digest, int hashlen );

unsigned int MD5_PseudoRandom(unsigned int nSeed);
*/

#endif // CHECKSUM_MD5_H
