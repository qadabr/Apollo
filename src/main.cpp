#include <stdio.h>

#include "Module.h"
#include "Server.h"
#include "Logger.h"

int main(int argc, char** argv)
{
	Server aServer;
	aServer.Listen();
	aServer.Accept();

	return 0;
}
