#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <thread>
#include <mutex>
#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "54000"
#define BUFFER_SIZE 512

#define BUFLEN 512	//Max length of buffer
#define PORT 8888	//The port on which to listen for incoming data

class TcpServer
{
public:
    static int setUpServer();

    static void initialiseSocket(int t_pos);

    static int setUpListenSocket();
    static void listenForConnections();
    static void listenOnSocket();

    static std::string recieveData(std::string t_data);

    static int listenForUDP();

    static SOCKET m_udpSocket;
    static std::mutex m_udpMutex;

    static SOCKET m_listenSocket;
    static std::vector<SOCKET> m_sockets;
    static std::vector<std::thread> m_threads;
    static std::vector<std::string> m_playerId;
    static bool debug;
};
