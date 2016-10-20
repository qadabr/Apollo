#include "SoundRecorder.h"
#include "SoundPlayer.h"

#include <cstdint>
#include <unistd.h>

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
		char* buffer = sr.DequeueBuffer();
		if (buffer != nullptr) {
			sp.EnqueueBuffer(buffer, sr.GetBufferSize());
			sleep(sr.GetChunkSize());
			delete[] buffer;
		}
	}
	
	return 0;
}
