#include <stdio.h>
#include <unistd.h>

#include "SilverPush.h"

int main(int argc, char** argv)
{
	std::string message = "333";
	
	SilverPush sp(500, 5000, 1000);
	sp.SendMessage(message);
	sleep(message.length() * 2);
	
	return 0;
}
