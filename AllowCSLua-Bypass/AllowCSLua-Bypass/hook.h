// by WALL-E
#pragma once

#ifndef __CVMTHOOK_H__
#define __CVMTHOOK_H__

#include <Windows.h>
#include <assert.h>

class CVMTManager
{
private:
	PVOID **m_Interface, *m_OldVMT, *m_NewVMT;
	int m_iCount;
public:
	CVMTManager( PVOID const thisptr, bool bHook = false )
	{
		this->m_Interface = reinterpret_cast< PVOID** >( thisptr );
		this->m_OldVMT = *this->m_Interface;
		if( !this->m_iCount )
			for( ; !IsBadCodePtr( reinterpret_cast< FARPROC >( this->m_OldVMT[this->m_iCount] ) ); ++this->m_iCount );
		this->m_NewVMT = new PVOID[this->m_iCount + 3];
		this->m_NewVMT[2] = this->m_OldVMT[-1];
		for( int i = 0; i < this->m_iCount; ++i )
			this->m_NewVMT[i + 3] = this->m_OldVMT[i];
		if( bHook )
			*this->m_Interface = &this->m_NewVMT[3];
	}
	~CVMTManager()
	{
		if( !IsBadCodePtr( reinterpret_cast< FARPROC >( this->m_Interface ) ) )
		{
			*this->m_Interface = this->m_OldVMT;
			delete [] this->m_NewVMT;
		}
	}
public:
	FORCEINLINE PVOID This()
	{
		assert( this->m_Interface );
		return this->m_Interface;
	}
	FORCEINLINE void HookMethod( PVOID newfn, int index )
	{
		assert( this->m_NewVMT && index <= this->m_iCount );
		this->m_NewVMT[index + 3] = newfn;
	}
	template< typename T >
	FORCEINLINE T GetMethod( int index ) const
	{
		assert( this->m_OldVMT && index <= this->m_iCount );
		return ( T )( this->m_OldVMT[index] );
	}
};

class CVMTHook
{
public:
	DWORD Hook ( DWORD new_Function, PDWORD pClass, int index )
	{
		m_NewFunction = new_Function;
		m_FunctionIndex = index;
		m_ClassTable = *( PDWORD* )pClass;
		m_OriginalFunction = m_ClassTable[ m_FunctionIndex ];
		m_FunctionPointer = &m_ClassTable[ m_FunctionIndex ];
		ReHook();
		return FunctionAddress();
	}

	void UnHook ( void )
	{
		MEMORY_BASIC_INFORMATION mbi;
		VirtualQuery( ( LPVOID )m_FunctionPointer, &mbi, sizeof( mbi ) );
		VirtualProtect( ( LPVOID )mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &mbi.Protect );
		m_ClassTable[ m_FunctionIndex ] = m_OriginalFunction;
		VirtualProtect( ( LPVOID )mbi.BaseAddress, mbi.RegionSize, mbi.Protect, NULL );
		FlushInstructionCache( GetCurrentProcess(), ( LPCVOID )m_FunctionPointer, sizeof( DWORD ) );
	}

	void ReHook ( void )
	{
		MEMORY_BASIC_INFORMATION mbi;
		VirtualQuery( ( LPVOID )m_FunctionPointer, &mbi, sizeof( mbi ) );
		VirtualProtect( ( LPVOID )mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &mbi.Protect );
		m_ClassTable[ m_FunctionIndex ] = m_NewFunction;
		VirtualProtect( ( LPVOID )mbi.BaseAddress, mbi.RegionSize, mbi.Protect, NULL );
		FlushInstructionCache( GetCurrentProcess(), ( LPCVOID )m_FunctionPointer, sizeof( DWORD ) );
	}

	DWORD FunctionAddress ( void )
	{
		return m_OriginalFunction;
	}

private:
	// Member variables
	int m_FunctionIndex;
	PDWORD m_ClassTable;
	PDWORD m_FunctionPointer;
	DWORD m_NewFunction;
	DWORD m_OriginalFunction;
};
#endif // __CVMTHOOK_H__