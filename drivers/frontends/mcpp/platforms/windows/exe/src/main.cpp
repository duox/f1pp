/*main.cpp*/
#include "pch.h"
#include "main.h"

int main( int argc, char ** argv )
{
	return f1::pp::frontends::mcpp::application().run( argc, argv );
}

#pragma comment(lib, "rpcrt4.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "version.lib")

/*END OF main.cpp*/
