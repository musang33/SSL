#include "stdafx.h"

#include "TestServer.h"

using namespace SSL;

int main( )
{
	TestServer testServer;
	if( testServer.Initialize( ) )
	{
		if( testServer.CreateListen( 10000, 100, 100, 10 ) )
		{
			while( 1 )
			{
				testServer.Update( );
			}			
		}
	}
}






