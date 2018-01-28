#ifndef ILUATABLE_H
#define ILUATABLE_H

class ILuaObject;

struct LuaKeyValue
{
	ILuaObject* pKey;
	ILuaObject* pValue;
};

#ifndef NO_SDK
	#include "tier1/utlvector.h"
	typedef CUtlVector<LuaKeyValue> CUtlLuaVector;

	#define FOR_LOOP( vecName, iteratorName ) \
	for ( int iteratorName = 0; iteratorName < tableMembers->Count(); iteratorName++ )
#else
	#include <vector>
	typedef std::vector<LuaKeyValue> CUtlLuaVector;

	#define FOR_LOOP( vecName, iteratorName ) \
	for ( unsigned int iteratorName = 0; iteratorName < tableMembers->size(); iteratorName++ )
#endif

#endif