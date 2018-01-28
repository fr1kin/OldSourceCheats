#ifndef __iluaobject__shit__shit
#define __iluaobject__shit__shit

#include "sdk.h"

#ifndef LUA_SHIT
#define LUA_SHIT
struct lua_State;

typedef int (*Lua_Func) (lua_State *L);

class ILuaInterface;
class ILuaShared;
#endif

extern void SetLuaInterface( ILuaInterface* pLua );

class ILuaObject
{
public:
	ILuaObject();
	ILuaObject( int, int );
	ILuaObject( ILuaObject* );
	~ILuaObject();

	virtual void Set( ILuaObject* );
	virtual int SetFromStack( int );
	virtual int UnReference( void );

	virtual int GetType( void );
	virtual const char* GetString( void );
	virtual float GetFloat( void );
	virtual int GetInt( void );
	virtual void* GetUserData( void );

	virtual void SetMember( const char* );
	virtual void SetMember( const char*, ILuaObject* );
	virtual void SetMember( const char*, float );
	virtual void SetMember( const char*, bool );
	virtual void SetMember( const char*, const char* );
	virtual void SetMember( const char*, Lua_Func );

	virtual bool GetMemberBool( const char*, bool );
	virtual int GetMemberInt( const char*, int );
	virtual float GetMemberFloat( const char*, float );
	virtual const char* GetMemberStr( const char*, const char* );
	virtual void* GetMemberUserData( const char*, void* );
	virtual void* GetMemberUserData( float, void* );
	virtual void GetMember( const char*, ILuaObject* );
	virtual void GetMember( ILuaObject*, ILuaObject* );

	virtual void SetMetaTable( ILuaObject* );
	virtual void SetUserData( void* );

	virtual void Push( void );

	virtual bool isNil( void );
	virtual bool isTable( void );
	virtual bool isString( void );
	virtual bool isNumber( void );
	virtual bool isFunction( void );
	virtual bool isUserData( void );

	virtual void GetMember( float, ILuaObject* );

	virtual void* Remove_Me_1( const char*, void* );

	virtual void SetMember( float );
	virtual void SetMember( float, ILuaObject* );
	virtual void SetMember( float, float );
	virtual void SetMember( float, bool );
	virtual void SetMember( float, const char* );
	virtual void SetMember( float, Lua_Func );

	virtual const char* GetMemberStr(float, const char* );

	virtual void SetMember( ILuaObject*, ILuaObject* );

	virtual bool GetBool( void );

	virtual bool PushMemberFast( int );

	virtual bool SetMemberFast( int, int );

	virtual void SetFloat( float );
	virtual void SetString( const char* );

	virtual double GetDouble( void );

	virtual void SetMember_FixKey( const char*, float );
	virtual void SetMember_FixKey( const char*, const char* );
	virtual void SetMember_FixKey( const char*, ILuaObject* );

	virtual bool isBool( void );

	virtual void SetMemberDouble( const char*, double );
	virtual void SetMemberNil( const char* );
	virtual void SetMemberNil( float );

	virtual bool Debug_IsUnreferenced( void );

	virtual ILuaObject* Init( void );

	virtual void SetFromGlobal( const char* );
	virtual void SetMember( const char*, unsigned long long );
	virtual int SetReference( int iRef = 1 );

	virtual void RemoveMember( const char* );
	virtual void RemoveMember( float );

	virtual bool MemberIsNil( const char* );

	virtual void SetMemberDouble( float, double );
	virtual double GetMemberDouble( const char* , double );
private:
	ILuaInterface* m_pLuaPtr;
	int m_iRef;
	int m_iType;
	bool m_bIsUserData;
};

#endif //__iluaobject__shit__shit