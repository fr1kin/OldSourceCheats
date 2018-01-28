#ifndef __NETVAR_H__
#define __NETVAR_H__

using namespace std;

typedef struct {
	RecvProp*				prop;
	string					propname;
	int						offset;
} netvar_prop_t;

typedef struct {
	string					classname;
	string					tablename;
	vector<netvar_prop_t>	props;
} netvar_class_t;

ClientClass* GetClientClass( void );

class CNetworkedVars
{
public:

	CNetworkedVars()
	{
		m_vNetVars.clear();
	}

	bool IsReady()
	{
		return ( m_vNetVars.size() > 0 );
	}

	void DumpTable( ClientClass* pClass, RecvTable* pTable )
	{
		if ( pTable->GetNumProps() <= 0 )
				return;
 
		if ( !Q_strstr( pTable->GetName(), "DT_" ) )
				return;
 
		netvar_class_t new_var_class;
 
		new_var_class.classname = pClass->GetName();
		new_var_class.tablename = pTable->GetName();
 
		for ( int i = 0 ; i < pTable->GetNumProps() ; i++ )
		{
				RecvProp* pProp = pTable->GetProp( i );
 
				if ( !pProp )
						continue;
 
				RecvTable* pChildTable = pProp->GetDataTable();
 
				if ( pChildTable && pChildTable->GetNumProps() > 0 )
						DumpTable( pClass, pChildTable );
 
				netvar_prop_t new_var_prop;
 
				new_var_prop.propname   = pProp->GetName();
				new_var_prop.offset             = pProp->GetOffset();
				new_var_prop.prop               = pProp;
 
				new_var_class.props.push_back( new_var_prop );
		}
		
		m_vNetVars.push_back( new_var_class );
	}

	bool StoreNetworkedVariableInfo( IBaseClientDLL* pClient )
	{
		if ( pClient == NULL )
                return false;
 
        m_vNetVars.clear();
 
        ClientClass *pClass = GetClientClass();
 
        for ( ; pClass ; pClass = pClass->m_pNext )
        {
                RecvTable *pTable = pClass->m_pRecvTable;
 
                DumpTable( pClass, pTable );
        }

		return IsReady();
	}

	netvar_class_t *GetClassInformation( string classname )
	{
		for( size_t i = 0; i < m_vNetVars.size(); i++ )
		{
			if( m_vNetVars[i].classname.compare( classname ) == 0 )
			{
				return &m_vNetVars[i];
			}
		}

		return NULL;
	}

	netvar_class_t *GetTableInformation( string tablename )
	{
		for( size_t i = 0; i < m_vNetVars.size(); i++ )
		{
			if( m_vNetVars[i].tablename.compare( tablename ) == 0 )
			{
				return &m_vNetVars[i];
			}
		}

		return NULL;
	}

	netvar_prop_t *GetPropFromClass( string classname, string propname )
	{
		netvar_class_t* pClass = GetClassInformation( classname );
		
		if( !pClass )
			return NULL;

		for( size_t i = 0; i < pClass->props.size(); i++ )
		{
			netvar_prop_t *pProp = &pClass->props[i];

			if( pProp )
			{
				if( pProp->propname.compare( propname ) == 0 )
				{
					return pProp;
				}
			}
		}

		return 0;
	}

	netvar_prop_t *GetPropFromTable( string tablename, string propname )
	{
		netvar_class_t* pClass = GetTableInformation( tablename );
		
		if( !pClass )
			return NULL;

		for( size_t i = 0; i < pClass->props.size(); i++ )
		{
			netvar_prop_t *pProp = &pClass->props[i];

			if( pProp )
			{
				if( pProp->propname.compare( propname ) == 0 )
				{
					return pProp;
				}
			}
		}

		return NULL;
	}

	RecvVarProxyFn HookClassProp( string classname, string propname, RecvVarProxyFn newProp )
	{
		netvar_prop_t *hook_prop = GetPropFromClass( classname, propname );

		if( !hook_prop )
			return NULL;

		return HookPropRecv( hook_prop->prop, newProp );
	}

	RecvVarProxyFn HookTableProp( string tablename, string propname, RecvVarProxyFn newProp )
	{
		netvar_prop_t *hook_prop = GetPropFromTable( tablename, propname );

		if( !hook_prop )
			return NULL;

		return HookPropRecv( hook_prop->prop, newProp );
	}

public:

	RecvVarProxyFn HookPropRecv( RecvProp* pProp, RecvVarProxyFn newProc )
	{
		RecvVarProxyFn store = pProp->GetProxyFn();

		pProp->SetProxyFn( newProc );

		return store;
	}

	vector<netvar_class_t>	m_vNetVars;
};

extern CNetworkedVars GNetworkedVariables;

#endif