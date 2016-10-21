#include "SilverPush.h"

#include <cstdint>
#include <unistd.h>

int main(int argc, char** argv)
{
	SoundEngine* engine = new SoundEngine();
	SilverPush silver(engine, 44100, 1000, 10000, 200);
	silver.ReceiveMessage();	
	
	return 0;
}
