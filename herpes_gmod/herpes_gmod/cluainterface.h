#ifndef __CLUAINTERFACE_
#define __CLUAINTERFACE_

#include "csdk.h"

class ILuaInterface
{
public:
	virtual int GetIRef( void ) = 0;

	virtual bool Init( void ) = 0;
	virtual void Shutdown( void ) = 0;
	virtual void Cycle( void ) = 0;

	virtual lua_State* GetLuaState( void ) = 0;
	virtual void Pop( int ) = 0;
	virtual void GetGlobal( ILuaObject*, char const* ) = 0;

	virtual ILuaObject* GetGlobal( char const* ) = 0;
	virtual ILuaObject* GetObject( int ) = 0;

	virtual const char* GetString( int ) = 0;
	virtual int GetInteger( int ) = 0;
	virtual float GetNumber( int ) = 0;
	virtual bool GetBool( int ) = 0;
	virtual void** GetUserDataPtr( int ) = 0;
	virtual void** GetUserData( int ) = 0;
	virtual void GetTable( int ) = 0;
	virtual int GetReference( int, bool ) = 0;

	virtual void FreeReference( int ) = 0;
	virtual void PushReference( int ) = 0;

	virtual void Push( ILuaObject * ) = 0;
	virtual void Push( char const* ) = 0;
	virtual void PushVA( char const*, ... ) = 0;
	virtual void Push( float ) = 0;
	virtual void Push( bool ) = 0;
	virtual void Push( int ( * )( lua_State * ) ) = 0;

	virtual void SetGlobal( char const*, ILuaObject * ) = 0;
	virtual void SetGlobal( char const*, bool ) = 0;
	virtual void SetGlobal( char const*, float ) = 0;
	virtual void SetGlobal( char const*, char  const* ) = 0;
	virtual void SetGlobal( char const*, CLuaFunction ) = 0;

	virtual void NewTable( void ) = 0;
	virtual void LuaError( char const*, int ) = 0;
	virtual void TypeError( char const*, int ) = 0;

	virtual int GetType( int ) = 0;
	virtual const char* GetTypeName( int ) = 0;

	virtual bool Call( int, int ) = 0;
	virtual bool Call( ILuaObject*, LArgList*, LArgList* ) = 0;
	virtual bool Call( ILuaObject*, LArgList*, ILuaObject* ) = 0;

	virtual void SetMember( ILuaObject*, char const* ) = 0;
	virtual void SetMember( ILuaObject*, char const*, ILuaObject* ) = 0;

	virtual int Top( void ) = 0;

	virtual ILuaObject* NewUserData( ILuaObject* ) = 0;
	virtual void PushUserData( ILuaObject*, void* ) = 0;

	virtual void NewGlobalTable( char const* ) = 0;
	virtual ILuaObject* NewTemporaryObject( void ) = 0;

	virtual void isUserData( int ) = 0;

	virtual ILuaObject* GetMetaTable( char const*, int );
	virtual ILuaObject* GetMetaTable( int ) = 0;
	virtual void SetMetaTable( ILuaObject* ,ILuaObject* ) = 0;

	virtual void CheckType( int, int ) = 0;
	virtual ILuaObject* GetReturn( int ) = 0;

	virtual bool IsServer( void ) = 0;
	virtual bool IsClient( void ) = 0;
	virtual bool IsDedicatedServer( void ) = 0;

	virtual void SetMember( ILuaObject*, float ) = 0;
	virtual void SetMember( ILuaObject*, float, ILuaObject* ) = 0;

	virtual ILuaObject* GetNewTable( void ) = 0;

	virtual void SetMember( ILuaObject*, ILuaObject*, ILuaObject* ) = 0;

	virtual void DebugPoint( void ) = 0;

	virtual ILuaModuleManager* GetModuleManager( void ) = 0;

	virtual void SetIsServer( bool ) = 0;

	virtual void PushLong( long ) = 0;
	virtual void PushValue( int ) = 0;
	virtual void PushNil( void ) = 0;

	virtual int GetFlags( int ) = 0;

	virtual bool FindOnObjectsMetaTable( int, int ) = 0;
	virtual bool FindObjectOnTable( int, int ) = 0;

	virtual void SetMemberFast( ILuaObject*, int, int ) = 0;

	virtual bool RunString( char const*, char const*, char const*, bool, bool ) = 0;

	virtual bool IsEqual( ILuaObject*, ILuaObject* ) = 0;

	virtual void Error( char const* ) = 0;

	virtual const char* GetStringOrError( int ) = 0;

	virtual bool RunModule( char const* ) = 0;

	virtual bool FindAndRunScript( char const*, bool, bool ) = 0;

	virtual void SetPathID( char const* ) = 0;
	virtual const char* GetPathID( void ) = 0;

	virtual void ErrorNoHalt( char const*, ... ) = 0;

	virtual int StringLength( int ) = 0;

	virtual void RemoveGlobal( char const* ) = 0;

	virtual int GetStackTop( void ) = 0;

	virtual CUtlLuaVector* GetAllTableMembers( int ) = 0;
	virtual void DeleteLuaVector( CUtlVector< LuaKeyValue, CUtlMemory< LuaKeyValue, int > >* ) = 0;

	virtual void Msg( char const*, ... ) = 0;

	virtual void PushPath( char const* ) = 0;
	virtual void PopPath( void ) = 0;
	virtual const char* GetPath( void ) = 0;

	virtual void ShouldTranslateLuaNames( void ) = 0;
	virtual bool SetShouldTranslateLuaNames( bool ) = 0;

	virtual void PushLightUserData( void* ) = 0;
	virtual void* GetLightUserData( int ) = 0;

	virtual void Lock( void ) = 0;
	virtual void UnLock( void ) = 0;

	virtual void SetGlobalDouble( char const*, double ) = 0;
	virtual double GetDouble( int ) = 0;
	virtual void PushDouble( double ) = 0;

	// Garry's "secret" scriptenforcer table.
	// Return types are unidentified, using void*.

	virtual void* GetStack( int, void* ) = 0;
	virtual void* GetInfo( char const*, void* ) = 0;
	virtual void* GetLocal( void*, int ) = 0;
	virtual void* GetUpvalue( int, int ) = 0;

	virtual void* CreateTable( int, int ) = 0;

	virtual void* DisableInfiniteLoopChecking( void ) = 0;

	virtual void* RunStringEx( char const*, char const*, char const*, bool, bool, bool ) = 0;

	virtual void* PushDataString( char const*, int ) = 0;

	virtual void* ResetMD5( void ) = 0;
	virtual void* MD5Changed( void ) = 0;
	virtual void* MD5Ack( int ) = 0;

	virtual void* RequireGlobal( char const*, char ) = 0;

	virtual void* ErrorFromLua( char const*, ... ) = 0;

	virtual void* SetTick( int ) = 0;

	virtual void* GetMD5String( void ) = 0;

	virtual void* GetCurrentLocation( void ) = 0;
};

class ILuaShared
{
public:
	virtual void Destructor() = 0;
	virtual void DumpStats(void) = 0;
	virtual void GetFile(char  const*,char  const*,bool,bool,bool *) = 0;
	virtual void FileExists(char  const*,char  const*,bool,bool,bool *) = 0;
	virtual void SetTranslateHook(void*) = 0;
	virtual void LZMACompress(unsigned char *,unsigned int) = 0;
	virtual void MountContent(void*) = 0;
	virtual void MountAddons(void*) = 0;
	virtual void MountGamemodes(void*) = 0;
	virtual void MountLua(char  const*,bool) = 0;
	virtual void MountLuaAdd(char  const*,char  const*) = 0;
	virtual void UnMountLua(char  const*) = 0;
	virtual void CloseLuaInterface(ILuaInterface *) = 0;
	virtual ILuaInterface* GetLuaInterface(unsigned char) = 0; // 13 / 14 w/e
	virtual void CreateLuaInterface(ILuaCallback *,unsigned char) = 0;
	virtual void GetAddonList(void) = 0;
	virtual void GetGamemodeList(void) = 0;
	virtual void Init(void * (*)(char  const*,int *),bool,void*) = 0;
	virtual void LoadCache(void) = 0;
	virtual void SaveCache(void) = 0;
	virtual void Shutdown(void) = 0;
	virtual void GetContentList(void) = 0;
	virtual void GetCommaSeperatedContentList(void) = 0;
	virtual void GetInterfaceByState(lua_State *) = 0;
	virtual void SetDepotMountable(char  const*,bool) = 0;
};

class ILuaModuleManager
{
public:
	virtual ILuaInterface* GetLuaInterface( lua_State * );
	virtual const char* GetBaseFolder( void );
};

#endif