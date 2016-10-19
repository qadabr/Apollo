#ifndef SOUNDRECORDER_H
#define SOUNDRECORDER_H

class SoundRecorder
{
 public:
	SoundRecorder();
	~SoundRecorder();
 private:
	SoundEngine* m_engine;
};

#endif /* SOUNDRECORDER_H */
