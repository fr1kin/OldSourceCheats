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
#endif // __CVMTHOOK_H__