class ILuaInterface;
#ifndef __vidja_luainterface_
#define __vidja_luainterface_

struct lua_State;

struct LoadS 
{
    const char *s;
    size_t size;
};

typedef const char* ( *lua_Reader_t )( lua_State *L, void *data, size_t *size );
typedef int ( __cdecl* lua_load_t )( lua_State *L, lua_Reader_t reader, LoadS* data, const char *chunkname );

typedef int (*CFunc) (lua_State *L);

enum LuaTypes
{
	TYPE_INVALID = -1,
	TYPE_NIL, 
	TYPE_BOOL,
	TYPE_LIGHTUSERDATA,
	TYPE_NUMBER, 
	TYPE_STRING, 
	TYPE_TABLE,
	TYPE_FUNCTION,
	TYPE_USERDATA,
	TYPE_THREAD,

	// UserData
	TYPE_ENTITY, 
	TYPE_VECTOR, 
	TYPE_ANGLE,
	TYPE_PHYSOBJ,
	TYPE_SAVE,
	TYPE_RESTORE,
	TYPE_DAMAGEINFO,
	TYPE_EFFECTDATA,
	TYPE_MOVEDATA,
	TYPE_RECIPIENTFILTER,
	TYPE_USERCMD,
	TYPE_SCRIPTEDVEHICLE,

	// Client Only
	TYPE_MATERIAL,
	TYPE_PANEL,
	TYPE_PARTICLE,
	TYPE_PARTICLEEMITTER,
	TYPE_TEXTURE,
	TYPE_USERMSG,

	TYPE_CONVAR,
	TYPE_IMESH,
	TYPE_MATRIX,
	TYPE_SOUND,
	TYPE_PIXELVISHANDLE,
	TYPE_DLIGHT,
	TYPE_VIDEO,
	TYPE_FILE,

	TYPE_COUNT
};

class ILuaInterface
{
public:
	virtual int Top(void);
	virtual void Push(int);
	virtual void Pop(int);	
	virtual void GetTable(int);
	virtual void GetField(int,const char*);
	virtual void SetField(int,const char*);
	virtual void CreateTable(void);
	virtual void SetTable(int);
	virtual void SetMetaTable(int);
	virtual void GetMetaTable(int);
	virtual void Call(int,int);
	virtual void PCall(int,int,int);
	virtual void Equal(int,int);
	virtual void RawEqual(int,int);
	virtual void Insert(int);
	virtual void Remove(int);
	virtual void Next(int);
	virtual void NewUserdata(unsigned int);
	virtual void ThrowError(const char*);
	virtual void CheckType(int,int);
	virtual void ArgError(int,const char*);
	virtual void RawGet(int);
	virtual void RawSet(int);
	virtual const char* GetString(int,unsigned int *);
	virtual int GetNumber(int);
	virtual bool GetBool(int);
	virtual void GetCFunction(int);
	virtual void* GetUserdata(int);
	virtual void PushNil(void);
	virtual void PushString(const char*,unsigned int);
	virtual void PushNumber(double);
	virtual void PushBool(bool);
	virtual void PushCFunction(CFunc) = 0;
	virtual void PushCClosure(void*,int);
	virtual void PushUserdata(void *);
	virtual int ReferenceCreate(void);
	virtual int ReferenceFree(int);
	virtual void ReferencePush(int);
	virtual void PushSpecial(int);
	virtual bool IsType(int,int);
	virtual int GetType(int);
	virtual const char* GetTypeName(int);
	virtual void CreateMetaTableType(const char*,int);
	virtual void CheckString(int);
	virtual void CheckNumber(int);
	virtual void ObjLen(int);
	virtual void AddThreadedCall(void*);
	virtual bool Init( void* pCallBack, bool bLoadJit );
	virtual void Shutdown(void);
	virtual void Cycle(void);
	virtual void* GetLuaState(void);
	virtual void* Global(void);
	virtual void GetObject(int);
	virtual void DELETE_ME_5437(int);
	virtual void _DELETE_ME2466(int);
	virtual void PushLuaObject(void* *);
	virtual void PushLuaFunction(void*);
	virtual void LuaError(const char*,int);
	virtual void TypeError(const char*,int);
	virtual void CallInternal(int,int);
	virtual void CallInternalNoReturns(int);
	virtual void CallInternalGetBool(int);
	virtual void CallInternalGetString(int);
	virtual void CallInternalGet(int,void* *);
	virtual void _DELETE_ME(void* *,void *);
	virtual void NewGlobalTable(const char*);
	virtual void NewTemporaryObject(void);
	virtual bool isUserData(int);
	virtual void GetMetaTableObject(const char*,int);
	virtual void GetMetaTableObject(int);
	virtual void GetReturn(int);
	virtual void IsServer(void);
	virtual void IsClient(void);
	virtual void IsDedicatedServer(void);
	virtual void DestroyObject(void*);
	virtual void CreateObject(void);
	virtual void SetMember(void*,void*,void*);
	virtual void GetNewTable(void);
	virtual void SetMember(void*,float);
	virtual void SetMember(void*,float,void*);
	virtual void SetMember(void*,const char*);
	virtual void SetMember(void*,const char*,void*);
	virtual void SetIsServer(bool);
	virtual void PushLong(long);
	virtual void GetFlags(int);
	virtual void FindOnObjectsMetaTable(int,int);
	virtual void FindObjectOnTable(int,int);
	virtual bool SetMemberFast(void*,int,int);
	virtual void RunString(const char*,const char*,const char*,bool,bool);
	virtual void IsEqual(void*,void* *);
	virtual void Error(const char*);
	virtual void GetStringOrError(int);
	virtual void RunLuaModule(const char*);
	virtual void FindAndRunScript(const char*,bool,bool,const char*);
	virtual void SetPathID(const char*);
	virtual void GetPathID(void);
	virtual void ErrorNoHalt(const char*,...);
	virtual void Msg(const char*,...);
	virtual void PushPath(const char*);
	virtual void PopPath(void);
	virtual void GetPath(void);
	virtual void GetColor(int);
	virtual void PushColor(int,int,int,int);
	virtual void GetStack(int,void *);
	virtual void GetInfo(const char*,void *);
	virtual void GetLocal(void *,int);
	virtual void GetUpvalue(int,int);
	virtual bool RunStringEx(const char*,const char*,const char*,bool,bool,bool);
	virtual void DELETE_ME1(const char*,int);
	virtual void GetDataString(int,void **);
	virtual void ErrorFromLua(const char*,...);
	virtual void GetCurrentLocation(void);
	virtual void MsgColour(void*,const char*,...);
	virtual void SetState(void*);
	virtual void DELETE_ME2(void);
	virtual void GetCurrentFile(void*);
	virtual void CompileString( void* , void* );
	virtual void ThreadLock(void);
	virtual void ThreadUnlock(void);
	virtual void CallFunctionProtected(int,int,bool);
	virtual void Require(const char*);
	virtual void GetActualTypeName(int);
	virtual void SetupInfiniteLoopProtection(void);
};

#endif