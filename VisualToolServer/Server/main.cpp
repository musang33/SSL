#include "stdafx.h"

#include "TestServer.h"

using namespace SSL;

int main( )
{	
	if( TestServer::GetInstance( )->Initialize( ) )
	{
		if( TestServer::GetInstance( )->CreateListen( 10000, 100, 100, 10 ) )
		{
			while( 1 )
			{
				TestServer::GetInstance( )->Update( );
			}			
		}
	}
}






