#include <stdio.h>
#include <unistd.h>

#include "SilverPush.h"

int main(int argc, char** argv)
{
	std::string message = "Hello";
	
	SilverPush sp(48000, 1000, 10000, 100);
	sp.SendMessage(message);
	sleep(message.length());
	
	return 0;
}
