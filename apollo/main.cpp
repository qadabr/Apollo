#include <stdio.h>
#include <unistd.h>

#include "SilverPush.h"

int main(int argc, char** argv)
{
	std::string message = "VLAD";
	
	SilverPush sp(4000, 10000, 500);
	sp.SendMessage(message);
	sleep(message.length());
	
	return 0;
}
