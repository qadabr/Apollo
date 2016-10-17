#include "Macros.h"
#include "Logger.h"

MODULE_INIT
{
	LOG_D("Привет, Horizon!\n");
	return 0;
}

MODULE_EXIT
{
	LOG_D("Прощай, Horizon :(\n");
}
