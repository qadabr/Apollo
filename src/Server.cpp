#include "Server.h"
using namespace std;

bool Server::g_isCreated = false;

Server::Server() : m_connectionThread(nullptr), m_isRunning(false)
{
	if (g_isCreated) {
		LOG_E("Server is already working on port %d\n",
		      SERVER_PORT);
		return;
	}

	m_listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	g_isCreated = true;
}

Server::~Server()
{
	Stop();
	g_isCreated = false;
}

void Server::Listen()
{
	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(SERVER_PORT);

	bind(m_listenSocket, (struct sockaddr*)&servaddr, sizeof(servaddr));
	listen(m_listenSocket, 5);

	LOG_D("Start listening on port %d\n", SERVER_PORT);
}

static void AcceptThread(int socket, bool isRunning)
{
	char message[SERVER_CHUNK];
	
	int connectionSocket = accept(socket, (struct sockaddr*)NULL, NULL);
	while (isRunning) {
		bzero(&message, SERVER_CHUNK);
		read(connectionSocket, message, SERVER_CHUNK);
		write(connectionSocket, message, SERVER_CHUNK);

		LOG_I("Received message: %s\n", message);

		this_thread::sleep_for(chrono::milliseconds(ACCEPT_THREAD_INTERVAL));
	}

	LOG_D("Accept thread is done\n");
}

void Server::Accept()
{
	m_isRunning = true;
	m_connectionThread = new std::thread(AcceptThread,
					     m_listenSocket,
					     m_isRunning);
}

void Server::Stop()
{
	m_isRunning = false;

	if (m_connectionThread != nullptr) {
		m_connectionThread->join();
		delete m_connectionThread;
		m_connectionThread = nullptr;
	}
}
