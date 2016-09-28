#include <stdio.h>

#include "Module.h"
#include "Logger.h"

int (*init)(void) = NULL;
void (*exit)(void) = NULL;

int main(int argc, char** argv) {
	Module modHelloWorld("HelloWorld.module");
	LOG_D("Try to load HelloWorld.module");
	modHelloWorld.Load();
	
	init = (typeof init)modHelloWorld.FindSymbol("mod_init");
	exit = (typeof exit)modHelloWorld.FindSymbol("mod_exit");

	if (init != NULL && init() == 0) {
		exit();
	}

	return 0;
}
