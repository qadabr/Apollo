#include <cstdint>
#include <unistd.h>

#include "SoundRecorder.h"
#include "SoundPlayer.h"

int main(int argc, char** argv)
{
	std::string message = "Hello";

	SoundEngine* engine = new SoundEngine();

	SoundRecorder sr(engine, 44100);
	SoundPlayer sp(engine, 44100);
	
	sp.ClearQueue();
	sp.Play();

	sr.ClearQueue();
	sr.Record();

	while (true) {
		char* buffer = sr.Dequeue();
		if (buffer != nullptr) {
			sp.EnqueueBuffer(buffer, sr.GetBufferSize());
		}
	}
	
	return 0;
}
