#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <netinet/in.h>
#include <strings.h>
#include <fcntl.h>
#include <thread>
#include <chrono>
#include <atomic>

#include "Setting.h"
#include "Logger.h"

#define ACCEPT_THREAD_INTERVAL 1000

class Server
{
 public:
	Server();
	~Server();
 public:
	void Listen();
	void Accept();
	void Stop();
 private:
	static bool                   g_isCreated;
	bool                          m_isRunning;
	int                           m_listenSocket;
	std::thread*                  m_connectionThread;
};

#endif /* SERVER_H */
