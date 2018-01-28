class CGMUserCmd;
class CVerifiedUserCmd;
#ifndef __vidja_cusercmd_
#define __vidja_cusercmd_

#include "sdk.h"

// 348 bytes

class CGMUserCmd
{
public:
	int		command_number;
	int		tick_count;
	float	viewangles[3];
	float	forwardmove;
	float	sidemove;      
	float	upmove;         
	int		buttons;
	BYTE    impulse;
	int		weaponselect;	
	int		weaponsubtype;
	int		random_seed;
	short	mousedx;
	short	mousedy;
	bool	hasbeenpredicted;
	char	entpad[20];
	char	pad01[4];
	short	pad02;
	char	pad03;
	char	pad04;
	float	pad05[3];
	char	pad06;
	char	pad10[240];
};

#endif