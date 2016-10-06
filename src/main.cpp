#include <stdio.h>

#include "Module.h"
#include "Logger.h"

int main(int argc, char** argv)
{
	Module modHelloWorld("HelloWorld.module");
	LOG_D("Try to load HelloWorld.module");

	modHelloWorld.Load();
	modHelloWorld.Start();
	modHelloWorld.Stop();

	return 0;
}
