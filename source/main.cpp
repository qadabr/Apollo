#include "SilverRay.h"

#include <cstdint>
#include <unistd.h>

int main(int argc, char** argv)
{
	SoundEngine* engine = new SoundEngine();

	SilverRay silver(engine, 44100, 18000, 19000, 200);
	silver.PushMessage("Hello!");
	silver.Send();
	//sleep(20);
	silver.ReceiveMessage(20000);
	
	return 0;
}
