#include "ILuaObject.h"

#define LUA_OBJECT_DEBUGMODE

ILuaInterface* pTEMP = NULL;

void SetLuaInterface( ILuaInterface* pLua )
{
	pTEMP = pLua;
}

ILuaObject::ILuaObject()
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::ILuaObject()" );
#endif
	this->m_pLuaPtr = pTEMP;
	this->m_iRef = -1;
	this->m_iType = -1;
	this->m_bIsUserData = false;
}

ILuaObject::ILuaObject( int iRef, int iType )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::ILuaObject( int iRef[%i], int iType[%i] )", iRef, iType );
#endif
	this->m_pLuaPtr = pTEMP;
	this->m_iRef = -1;
	this->m_iType = -1;
	this->m_bIsUserData = false;

	this->SetReference( iRef );
}

ILuaObject::ILuaObject( ILuaObject* pObj )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::ILuaObject( ILuaObject* pObj[0x%X] )", ( DWORD )( pObj ) );
#endif
	this->m_pLuaPtr = pTEMP;
	this->m_iRef = -1;
	this->m_iType = -1;
	this->m_bIsUserData = false;
	if( this->m_pLuaPtr )
	{
		this->SetFromStack( -1 );
		this->m_pLuaPtr->Pop( 1 );
	}
}

ILuaObject::~ILuaObject()
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::~ILuaObject()" );
#endif
	if( this->m_iRef != -1 && this->m_pLuaPtr )
	{
		this->m_pLuaPtr->ReferenceFree( this->m_iRef );
	}
}

void ILuaObject::Set( ILuaObject* pObj )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::Set( ILuaObject* pObj[0x%X] )", ( DWORD )( pObj ) );
#endif
	if( pObj )
	{
		pObj->Push();
		this->SetFromStack( -1 );
		return this->m_pLuaPtr->Pop( 1 );
	}
	this->UnReference();
}

int ILuaObject::SetFromStack( int iRef )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::SetFromStack( int iRef[%i] )", iRef );
#endif
	return this->SetReference( iRef );
}

int ILuaObject::UnReference( void )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::UnReference()" );
#endif
	int ret = this->m_iRef;
	if( ret != -1 )
		ret = this->m_pLuaPtr->ReferenceFree( this->m_iRef );
	this->m_bIsUserData = false;
	this->m_iRef = -1;
	this->m_iType = -1;
	return ret;
}

int ILuaObject::GetType( void )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::GetType()" );
#endif
	if( !this->m_pLuaPtr )
		this->UnReference();
	return this->m_iType;
}

const char* ILuaObject::GetString( void )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::GetString()" );
#endif
	this->Push();
	const char* ret = this->m_pLuaPtr->GetString( -1, 0 );
	this->m_pLuaPtr->Pop( 1 );
	return ret;
}

float ILuaObject::GetFloat( void )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::GetFloat()" );
#endif
	if( this->isNumber() )
	{
		this->Push();
		float ret = ( float )( this->m_pLuaPtr->GetNumber( -1 ) );
		this->m_pLuaPtr->Pop( 1 );
		return ret;
	}
	return NULL;
}

int ILuaObject::GetInt( void )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::GetInt()" );
#endif
	if( this->isNumber() )
	{
		this->Push();
		int ret = this->m_pLuaPtr->GetNumber( -1 );
		this->m_pLuaPtr->Pop( 1 );
		return ret;
	}
	return NULL;
}

void* ILuaObject::GetUserData( void )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::GetUserData()" );
#endif
	if( this->isUserData() )
	{
		this->Push();
		void* ret = this->m_pLuaPtr->GetUserdata( -1 );
		this->m_pLuaPtr->Pop( 1 );
		return ret;
	}
	return NULL;
}

void ILuaObject::SetMember( const char* pszName )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::SetMember( const char* pszName[%s] )", pszName );
#endif
	DebugMessage( "1] Lua Top %i", this->m_pLuaPtr->Top() );
	//this->m_pLuaPtr->SetMember( this, pszName );
	if( this->isTable() )
	{
		this->Push();
		this->m_pLuaPtr->PushString( pszName, 0 );
		this->m_pLuaPtr->Push( -3 );
		this->m_pLuaPtr->SetTable( -3 );
		this->m_pLuaPtr->Pop( 1 );
	}
	DebugMessage( "2] Lua Top %i", this->m_pLuaPtr->Top() );
}

void ILuaObject::SetMember( const char* pszName, ILuaObject* pValue )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::SetMember( const char* pszName[%s], ILuaObject* pValue[0x%X] )", pszName, pValue );
#endif
	return this->m_pLuaPtr->SetMember( this, pszName, pValue );
}

void ILuaObject::SetMember( const char* pszName, float flValue )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::SetMember( const char* pszName[%s], float flValue[%f] )", pszName, flValue );
#endif
	if( this->isTable() )
	{
		this->m_pLuaPtr->PushNumber( flValue );
		return this->SetMember( pszName );
	}
	return;
}

void ILuaObject::SetMember( const char* pszName, bool bValue )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::SetMember( const char* pszName[%s], bool bValue[%s] )", pszName, bValue ? "true" : "false" );
#endif
	if( this->isTable() )
	{
		this->m_pLuaPtr->PushBool( bValue );
		return this->SetMember( pszName );
	}
	return;
}

void ILuaObject::SetMember( const char* pszName, const char* pszValue )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::SetMember( const char* pszName[%s], const char* pszValue[%s] )", pszName, pszValue );
#endif
	if( this->isTable() )
	{
		this->m_pLuaPtr->PushString( pszValue, 0 );
		return this->SetMember( pszName );
	}
	return;
}

void ILuaObject::SetMember( const char* pszName, Lua_Func lcFunc )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::SetMember( const char* pszName[%s], LuaFunc lcFunc[0x%X] )", pszName, ( DWORD )( lcFunc ) );
#endif
	if( this->isTable() )
	{
		this->m_pLuaPtr->PushLuaFunction( lcFunc );
		return this->SetMember( pszName );
	}
	return;
}

bool ILuaObject::GetMemberBool( const char* pszName, bool bValue )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::GetMemberBool( const char* pszName[%s], bool bValue[%s] )", pszName, bValue ? "true" : "false" );
#endif
	bool ret = bValue;
	if( this->isTable() && this->GetType() == 5 )
	{
		this->Push();
		this->m_pLuaPtr->PushString( pszName, 0 );
		this->m_pLuaPtr->GetTable( -2 );
		if( this->m_pLuaPtr->GetType( -1 ) == 1 )
			ret = this->m_pLuaPtr->GetBool( -1 );
		this->m_pLuaPtr->Pop( 2 );
	}
	return ret;
}

int ILuaObject::GetMemberInt( const char* pszName, int iValue )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::GetMemberInt( const char* pszName[%s], int iValue[%i] )", pszName, iValue );
#endif
	int ret = iValue;
	if( this->isTable() )
	{
		this->Push();
		this->m_pLuaPtr->PushString( pszName, 0 );
		this->m_pLuaPtr->GetTable( -2 );
		if( this->m_pLuaPtr->GetType( -1 ) == 3 )
			ret = this->m_pLuaPtr->GetNumber( -1 );
		this->m_pLuaPtr->Pop( 2 );
	}
	return ret;
}

float ILuaObject::GetMemberFloat( const char* pszName, float flValue )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::GetMemberFloat( const char* pszName[%s], float flValue[%f] )", pszName, flValue );
#endif
	float ret = flValue;
	if( this->isTable() )
	{
		this->Push();
		this->m_pLuaPtr->PushString( pszName, 0 );
		this->m_pLuaPtr->GetTable( -2 );
		if( this->m_pLuaPtr->GetType( -1 ) == 3 )
			ret = ( float )this->m_pLuaPtr->GetNumber( -1 );
		this->m_pLuaPtr->Pop( 2 );
	}
	return ret;
}

const char* ILuaObject::GetMemberStr( const char* pszName, const char* pszValue )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::GetMemberStr( const char* pszName[%s], const char* pszValue[%s] )", pszName, pszValue );
#endif
	const char* ret = pszValue;
	if( this->isTable() )
	{
		this->Push();
		this->m_pLuaPtr->PushString( pszName, 0 );
		this->m_pLuaPtr->GetTable( -2 );
		if( this->m_pLuaPtr->GetType( -1 ) == 4 )
			ret = this->m_pLuaPtr->GetString( -1, 0 );
		this->m_pLuaPtr->Pop( 2 );
	}
	return ret;
}

void* ILuaObject::GetMemberUserData( const char* pszName, void* pValue )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::GetMemberUserData( const char* pszName[%s], void* pValue[0x%X] )", pszName, ( DWORD )( pValue ) );
#endif
	void* ret = pValue;
	if( this->isTable() )
	{
		this->Push();
		this->m_pLuaPtr->PushString( pszName, 0 );
		this->m_pLuaPtr->GetTable( -2 );
		if( this->m_pLuaPtr->isUserData( -1 ) )
			ret = this->m_pLuaPtr->GetUserdata( -1 );
		this->m_pLuaPtr->Pop( 2 );
	}
	return ret;
}

void* ILuaObject::GetMemberUserData( float flPush, void* pValue )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::GetMemberUserData( float flPush[%f], void* pValue[0x%X] )", flPush, ( DWORD )( pValue ) );
#endif
	void* ret = pValue;
	if( this->isTable() )
	{
		this->Push();
		this->m_pLuaPtr->PushNumber( flPush );
		this->m_pLuaPtr->GetTable( -2 );
		if( this->m_pLuaPtr->isUserData( -1 ) )
			ret = this->m_pLuaPtr->GetUserdata( -1 );
		this->m_pLuaPtr->Pop( 2 );
	}
	return ret;
}

void ILuaObject::GetMember( const char* pszName, ILuaObject* pObj )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::GetMember( const char* pszName[%s], ILuaObject* pObj[0x%X] )", pszName, ( DWORD )( pObj ) );
#endif
	if( this->isTable() )
	{
		this->Push();
		this->m_pLuaPtr->PushString( pszName, 0 );
		this->m_pLuaPtr->GetTable( -2 );
		this->SetFromStack( -1 );
		return this->m_pLuaPtr->Pop( 2 );
	}
	return;
}

void ILuaObject::GetMember( ILuaObject* pObj, ILuaObject* pObjMemb )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::GetMember( ILuaObject* pObj[0x%X], ILuaObject* pObjMemb[0x%X] )", ( DWORD )( pObj ), ( DWORD )( pObjMemb ) );
#endif
	if( this->isTable() )
	{
		this->Push();
		pObj->Push();
		this->m_pLuaPtr->GetTable( -2 );
		if( this->m_pLuaPtr->GetType( -1 ) )
			pObjMemb->SetFromStack( -1 );
		return this->m_pLuaPtr->Pop( 2 );
	}
	return;
}

void ILuaObject::SetMetaTable( ILuaObject* pObj )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::SetMetaTable( ILuaObject* pObj[0x%X] )", ( DWORD )( pObj ) );
#endif
	this->Push();
	pObj->Push();
	this->m_pLuaPtr->GetTable( -2 );
	return this->m_pLuaPtr->Pop( 1 );
}

void ILuaObject::SetUserData( void* pData )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::SetMetaTable( void* pData[0x%X] )", ( DWORD )( pData ) );
#endif
	this->Push();
	void* pUserData = this->m_pLuaPtr->GetUserdata( -1 );
	if( pUserData )
		pUserData = pData;
	this->m_pLuaPtr->Pop( 1 );
}

void ILuaObject::Push( void )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::Push()" );
#endif
	if( this->GetType() == -1 )
		return this->m_pLuaPtr->PushNil();
	return this->m_pLuaPtr->ReferencePush( this->m_iRef );
}

bool ILuaObject::isNil( void )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::isNil()" );
#endif
	return this->GetType() == 0 || this->GetType() == -1;
}

bool ILuaObject::isTable( void )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::isTable()" );
#endif
	return this->GetType() == 5;
}

bool ILuaObject::isString( void )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::isString()" );
#endif
	return this->GetType() == 4;
}

bool ILuaObject::isNumber( void )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::isNumber()" );
#endif
	return this->GetType() == 3;
}

bool ILuaObject::isFunction( void )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::isFunction()" );
#endif
	return this->GetType() == 6;
}

bool ILuaObject::isUserData( void )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::isUserData()" );
#endif
	return this->m_bIsUserData;
}

void ILuaObject::GetMember( float flPush, ILuaObject* pObj )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::GetMember( float flPush[%f], ILuaObject* pObj[0x%X] )", flPush, ( DWORD )( pObj ) );
#endif
	if( this->isTable() )
	{
		this->Push();
		this->m_pLuaPtr->PushNumber( flPush );
		this->m_pLuaPtr->GetTable( -2 );
		pObj->SetFromStack( -1 );
		return this->m_pLuaPtr->Pop( 2 );
	}
	return;
}

void* ILuaObject::Remove_Me_1( const char* pszName, void* pData )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::Remove_Me_1( const char* pszName[%s], void* pData[0x%X] )", pszName, ( DWORD )( pData ) );
#endif
	return pData;
}

void ILuaObject::SetMember( float flPush )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::SetMember( float flPush[%f] )", flPush );
#endif
	return this->m_pLuaPtr->SetMember( this, flPush );
}

void ILuaObject::SetMember( float flPush, ILuaObject* pObj )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::SetMember( float flPush[%f], ILuaObject* pObj[0x%X] )", flPush, ( DWORD )( pObj ) );
#endif
	return this->m_pLuaPtr->SetMember( this, flPush, pObj );
}

void ILuaObject::SetMember( float flPush, float flValue )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::SetMember( float flPush[%f], float flValue[%f] )", flPush, flValue );
#endif
	if( this->isTable() )
	{
		this->m_pLuaPtr->PushNumber( flValue );
		return this->SetMember( flPush );
	}
	return;
}

void ILuaObject::SetMember( float flPush, bool bValue )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::SetMember( float flPush[%f], bool bValue[%s] )", flPush, bValue ? "true" : "false" );
#endif
	if( this->isTable() )
	{
		this->m_pLuaPtr->PushBool( bValue );
		return this->SetMember( flPush );
	}
	return;
}

void ILuaObject::SetMember( float flPush, const char* pszValue )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::SetMember( float flPush[%f], const char* pszValue[%s] )", flPush, pszValue );
#endif
	if( this->isTable() )
	{
		this->m_pLuaPtr->PushString( pszValue, 0 );
		return this->SetMember( flPush );
	}
	return;
}

void ILuaObject::SetMember( float flPush, Lua_Func fnValue )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::SetMember( float flPush[%f], Lua_Func fnValuee[0x%X] )", flPush, ( DWORD )( fnValue ) );
#endif
	if( this->isTable() )
	{
		this->m_pLuaPtr->PushCFunction( fnValue );
		return this->SetMember( flPush );
	}
	return;
}

const char* ILuaObject::GetMemberStr( float flPush, const char* pszValue )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::GetMemberStr( float flPush[%f], const char* pszValue[%s] )", flPush, pszValue );
#endif
	const char* ret = pszValue;
	if( this->isTable() )
	{
		this->Push();
		this->m_pLuaPtr->PushNumber( flPush );
		this->m_pLuaPtr->GetTable( -2 );
		if( this->m_pLuaPtr->GetType( -1 ) == 4 )
			ret = this->m_pLuaPtr->GetString( -1, 0 );
		this->m_pLuaPtr->Pop( 2 );
	}
	return ret;
}

void ILuaObject::SetMember( ILuaObject* pObj, ILuaObject* pObjValue )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::SetMember( ILuaObject* pObj[0x%X], ILuaObject* pObjValue[0x%X] )", ( DWORD )( pObj ), ( DWORD )( pObjValue ) );
#endif
	return this->m_pLuaPtr->SetMember( this, pObj, pObjValue );
}

bool ILuaObject::GetBool( void )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::GetBool()" );
#endif
	bool ret = false;
	if( this->isBool() )
	{
		this->Push();
		ret = this->m_pLuaPtr->GetBool( -1 );
		this->m_pLuaPtr->Pop( 1 );
	}
	return ret;
}

bool ILuaObject::PushMemberFast( int iValue )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::PushMemberFast( int iValue[%i] )", iValue );
#endif
	bool ret = false;
	if( this->isTable() )
	{
		this->Push();
		this->m_pLuaPtr->Push( iValue );
		this->m_pLuaPtr->GetTable( -2 );
		ret = true;
		if( this->m_pLuaPtr->IsType( -1, 0 ) )
			ret = false;
		this->m_pLuaPtr->Pop( 1 );
	}
	return ret;
}

bool ILuaObject::SetMemberFast( int iKey, int iValue )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::SetMemberFast( int iKey[%i], int iValue[%i] )", iKey, iValue );
#endif
	bool ret = this->isTable();
	if( ret )
		ret = this->m_pLuaPtr->SetMemberFast( this, iKey, iValue );
	return ret;
}

void ILuaObject::SetFloat( float flKey )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::SetFloat( float iKey[%f] )", flKey );
#endif
	this->m_pLuaPtr->PushNumber( flKey );
	this->SetFromStack( -1 );
	return this->m_pLuaPtr->Pop( 1 );
}

void ILuaObject::SetString( const char* pszKey )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::SetString( const char* pszKey[%s] )", pszKey );
#endif
	this->m_pLuaPtr->PushString( pszKey, 0 );
	this->SetFromStack( -1 );
	return this->m_pLuaPtr->Pop( 1 );
}

double ILuaObject::GetDouble( void )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::GetDouble()" );
#endif
	double ret = 0.0;
	if( this->isNumber() )
	{
		this->Push();
		ret = ( double )( this->m_pLuaPtr->GetNumber( -1 ) );
		this->m_pLuaPtr->Pop( 1 );
	}
	return ret;
}

void ILuaObject::SetMember_FixKey( const char* pszKey, float flValue )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::SetMember_FixKey( const char* pszKey[%s], float flValue[%f] )", pszKey, flValue );
#endif
	double dbKey = atof( pszKey );
	if( dbKey == 0.0 )
		return this->SetMember( pszKey, flValue );
	return this->SetMember( ( float )( dbKey ), flValue );
}

void ILuaObject::SetMember_FixKey( const char* pszKey, const char* pszValue )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::SetMember_FixKey( const char* pszKey[%s], const char* pszValue[%s] )", pszKey, pszValue );
#endif
	double dbKey = atof( pszKey );
	if( dbKey == 0.0 )
		return this->SetMember( pszKey, pszValue );
	return this->SetMember( ( float )( dbKey ), pszValue );
}

void ILuaObject::SetMember_FixKey( const char* pszKey, ILuaObject* pValue )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::SetMember_FixKey( const char* pszKey[%s], ILuaObject* pValue[0x%X] )", pszKey, ( DWORD )( pValue ) );
#endif
	double dbKey = atof( pszKey );
	if( dbKey == 0.0 )
		return this->SetMember( pszKey, pValue );
	return this->SetMember( ( float )( dbKey ), pValue );
}

bool ILuaObject::isBool( void )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::isBool()" );
#endif
	return this->GetType() == 1;
}

void ILuaObject::SetMemberDouble( const char* pszKey, double dbValue )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::SetMemberDouble( const char* pszKey[%s], double dbValue[%f] )", pszKey, dbValue );
#endif
	if( this->isTable() )
	{
		this->m_pLuaPtr->PushNumber( dbValue );
		return this->SetMember( pszKey );
	}
	return;
}

void ILuaObject::SetMemberNil( const char* pszKey )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::SetMemberNil( const char* pszKey[%s] )", pszKey );
#endif
	this->m_pLuaPtr->PushNil();
	return this->SetMember( pszKey );
}

void ILuaObject::SetMemberNil( float flKey )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::SetMemberNil( float flKey[%f] )", flKey );
#endif
	this->m_pLuaPtr->PushNil();
	return this->SetMember( flKey );
}

bool ILuaObject::Debug_IsUnreferenced( void )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::Debug_IsUnreferenced()" );
#endif
	return this->GetType() == -1;
}

ILuaObject* ILuaObject::Init( void )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::Init()" );
#endif
	this->m_iRef = -1;
	this->m_iType = -1;
	this->m_bIsUserData = false;
	this->m_pLuaPtr = pTEMP;
	return this;
}

void ILuaObject::SetFromGlobal( const char* pszKey )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::SetFromGlobal( const char* pszKey[%s] )", pszKey );
#endif
	ILuaObject* pGlobal = ( ILuaObject* )( this->m_pLuaPtr->Global() );
	return pGlobal->GetMember( pszKey, this );
}

void ILuaObject::SetMember( const char* pszKey, unsigned long long lValue )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::SetMember( const char* pszKey[%s], unsigned long long lValue[%f] )", pszKey, lValue );
#endif
	if( this->isTable() )
	{
		char temp[256];
		sprintf_s( temp, "%llu", lValue );
		this->m_pLuaPtr->PushString( temp, 0 );
		return this->m_pLuaPtr->SetMember( this, pszKey );
	}
	return;
}

int ILuaObject::SetReference( int iRef )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::SetReference( int iRef[%i] )", iRef );
#endif
	this->UnReference();
	this->m_pLuaPtr = pTEMP;
	this->m_bIsUserData = this->m_pLuaPtr->isUserData( iRef );
	this->m_iType = this->m_pLuaPtr->GetType( iRef );
	if( this->m_iType )
		this->m_pLuaPtr->Push( iRef );
	else
		this->m_pLuaPtr->PushNil();
	this->m_iRef = this->m_pLuaPtr->ReferenceCreate();
	DebugMessage( "Ref: %i", this->m_iRef );
	return this->m_iRef;
}

void ILuaObject::RemoveMember( const char* pszMember )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::RemoveMember( const char* pszMember[%s] )", pszMember );
#endif
	this->m_pLuaPtr->PushNil();
	this->m_pLuaPtr->SetMember( this, pszMember );
}

void ILuaObject::RemoveMember( float flMember )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::RemoveMember( float flMember[%f] )", flMember );
#endif
	this->m_pLuaPtr->PushNil();
	this->m_pLuaPtr->SetMember( this, flMember );
}

bool ILuaObject::MemberIsNil( const char* pszMember )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::RemoveMember( const char* pszMember[%s] )", pszMember );
#endif
	this->Push();
	this->m_pLuaPtr->PushString( pszMember, 0 );
	this->m_pLuaPtr->GetTable( -2 );
	int ret = this->m_pLuaPtr->GetType( -1 );
	this->m_pLuaPtr->Pop( 2 );
	return ( ret == 0 );
}

void ILuaObject::SetMemberDouble( float flKey, double dbValue )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::SetMemberDouble( float flKey[%f], double dbValue[%f] )", flKey, dbValue );
#endif
	if( this->isTable() )
	{
		this->m_pLuaPtr->PushNumber( dbValue );
		this->SetMember( flKey );
	}
	return;
}

double ILuaObject::GetMemberDouble( const char* pszKey, double dbValue )
{
#ifdef LUA_OBJECT_DEBUGMODE
	DebugMessage( "ILuaObject::SetMemberDouble( const char* pszKey[%s], double dbValue[%f] )", pszKey, dbValue );
#endif
	double ret = 0.0;
	if( this->isTable() )
	{
		//this->m_pLuaPtr->PushNumber( dbValue );
		//ret = this->SetMember( pszKey );
	}
	return ret;
}