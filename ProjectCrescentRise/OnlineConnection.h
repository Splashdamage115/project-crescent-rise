#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <thread>
#include <mutex>
#include <queue>
#include "OnlineMover.h"
#include "GameObjects.h"
#pragma comment(lib, "Ws2_32.lib")

#define SERVER_IP "127.0.0.1"
//#define SERVER_IP "149.153.106.6"
#define DEFAULT_PORT "54000"
#define UDP_PORT 8888
#define BUFFER_SIZE 512

class OnlineConnection
{
public:
	~OnlineConnection();
	static int Start();

	static void pushTcp(std::string& data);
	static void pushPosition(std::string& data);
	static void pullPosition(std::string positionData);
	static int playerId;
private:
	static int initiliseDataFromServer();
	static int SetUpConnections();
	static int SetUpTcp();
	static int SetUpUdp();
	static void LoopConnection();
	static bool popTcp(std::string& data);
	static void addNewPlayer();

	static bool loopActive;

	static SOCKET tcpSocket;
	static SOCKET udpSocket;

	static std::queue<std::string> tcpQueue;
	static std::queue<std::string> recievedData;
	static std::mutex tcpMutex;
	static std::mutex playersMutex;
	static std::string position;
	static std::mutex positionMutex;

	static std::vector<std::pair<int, std::shared_ptr<OnlineMover>>> m_OnlinePlayer;
	static std::vector<std::shared_ptr<GameObject>> m_onlinePlayers;

	static std::atomic<bool> m_readyToCreate;
	static std::atomic<int> m_createPosition;
	static std::atomic<bool> m_creating;
};
