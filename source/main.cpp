#include "SilverPush.h"

#include <cstdint>
#include <unistd.h>

int main(int argc, char** argv)
{
	SoundEngine* engine = new SoundEngine();

	// 85 миллисекунд -- идеальное значения для расчета преобразования Фурье
	SilverPush silver(engine, 48000, 19000, 20000, 85);
	silver.PushMessage("Hello World!");
	silver.Send();
	silver.ReceiveMessage();
	
	return 0;
}
