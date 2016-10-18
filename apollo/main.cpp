#include <stdio.h>

#include "Logger.h"
#include "SoundPlayer.h"

int main(int argc, char** argv)
{
	SoundPlayer player;
	player.PlayTone(1000, 3);

	return 0;
}
