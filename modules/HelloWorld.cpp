#include "Logger.h"
#include <android/log.h>

extern "C" int mod_init(void)
{
	LOG_D("Привет, Horizon!\n");
	return 0;
}

extern "C" void mod_exit(void)
{
	LOG_D("Прощай, Horizon :(\n");
}
