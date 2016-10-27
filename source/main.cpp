#include "SilverPush.h"

#include <cstdint>
#include <unistd.h>

int main(int argc, char** argv)
{
	SoundEngine* engine = new SoundEngine();

	SilverPush silver(engine, 48000, 21500, 22000, 100);
	silver.PushMessage("Hello!");
	silver.Send();
	silver.ReceiveMessage();
	
	return 0;
}
