#include "SilverPush.h"

#include <cstdint>
#include <unistd.h>

int main(int argc, char** argv)
{
	SoundEngine* engine = new SoundEngine();
	
	SilverPush silver(engine, 48000, 17000, 20000, 200);
	silver.SendMessage("Hi World");
	silver.ReceiveMessage();	
	
	return 0;
}
