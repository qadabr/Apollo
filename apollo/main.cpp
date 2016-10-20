#include <stdio.h>
#include <unistd.h>

#include "SoundRecorder.h"

int main(int argc, char** argv)
{
	std::string message = "Hello";

	SoundEngine* engine = new SoundEngine();
	SoundRecorder sr(engine, 44100);
	sr.Record();

	sleep(5);
	
	return 0;
}
