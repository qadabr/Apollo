#include "Logger.h"
#include <android/log.h>

int mod_init(void)
{
	LOG_D("Привет, Horizon!\n");
	return 0;
}

void mod_exit(void)
{
	LOG_D("Прощай, Horizon :(\n");
}
