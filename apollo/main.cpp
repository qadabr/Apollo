#include <cstdint>

#include "SoundRecorder.h"
#include "SoundPlayer.h"

int main(int argc, char** argv)
{
	std::string message = "Hello";

	SoundEngine* engine = new SoundEngine();

	SoundRecorder sr(engine, 48000);
	SoundPlayer sp(engine, 48000);

	sr.Record();
	
	sp.ClearQueue();
	sp.Play();

	int16_t* buffer = sr.Dequeue();
	while (true) {
		sp.EnqueueBuffer((char*)buffer, sr.GetBufferSize() * sizeof(int16_t));
	}
	
	return 0;
}
