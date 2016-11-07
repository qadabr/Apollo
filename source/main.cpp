#include "SilverRay.h"

#include <cstdint>
#include <unistd.h>

int main(int argc, char** argv)
{
	SoundEngine* engine = new SoundEngine();

	SilverRay silver(engine, 44100, 18500, 19500, 200);
	silver.PushMessage("How are you today?");
	silver.Send();
	//sleep(30);
	silver.ReceiveMessage(30000);
	
	return 0;
}
