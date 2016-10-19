#include <stdio.h>
#include <unistd.h>

#include "SilverPush.h"

int main(int argc, char** argv)
{
	std::string message = "VLAD";
	
	SilverPush sp(48000, 5000, 10000, 200);
	sp.SendMessage(message);
	sleep(message.length());
	
	return 0;
}
