#include "SilverPush.h"

#include <cstdint>
#include <unistd.h>

int main(int argc, char** argv)
{
	SoundEngine* engine = new SoundEngine();

	// 85 миллисекунд -- идеальное значения для расчета преобразования Фурье для фрейма
	SilverPush silver(engine, 48000, 19000, 20000, 200);
	silver.PushMessage("How are you?");
	silver.Send();
	silver.ReceiveMessage(20000);
	
	return 0;
}
