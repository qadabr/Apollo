#include "SilverPush.h"

#include <cstdint>
#include <unistd.h>

int main(int argc, char** argv)
{
	SoundEngine* engine = new SoundEngine();

	SilverPush silver(engine, 48000, 19000, 20000, 50);
	silver.PushMessage("hello");
	silver.Send();
	silver.ReceiveMessage();
	
	return 0;
}
