#include "SDK.h"

ClientClass* GetClientClass( void )
{
	ClientClass* pClass = NULL;

	DWORD* pdwClientVMT = ( DWORD* )*( DWORD* )HERPES::Interfaces::m_pClient;
	DWORD dwGetAllClasses = pdwClientVMT[8];

	__asm
	{
		PUSHAD;
		CALL dwGetAllClasses;
		MOV pClass, EAX;
		POPAD;
	}

	return pClass;
}

CNetworkedVars GNetworkedVariables;