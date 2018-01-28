class ILuaShared;
#ifndef __vidja_luashared_
#define __vidja_luashared_

#include "luainterface.h"

class ILuaShared
{
public:
	virtual void Destructor();
	virtual void Init(void*,bool,void* *,void*);
	virtual void Shutdown(void);
	virtual void DumpStats(void);
	virtual ILuaInterface* CreateLuaInterface(unsigned char,bool);
	virtual int CloseLuaInterface( ILuaInterface* );
	virtual ILuaInterface* GetLuaInterface(unsigned char);
	virtual void LoadFile(void*,void*,bool,bool);
	virtual void GetCache(void*);
	virtual void MountLua(char  const*);
	virtual void MountLuaAdd(char  const*,char  const*);
	virtual void UnMountLua(char  const*);
	virtual void SetFileContents(char  const*,char  const*);
	virtual void SetLuaFindHook(void *);
	virtual void FindScripts(void*,void*,void*);
};


#endif